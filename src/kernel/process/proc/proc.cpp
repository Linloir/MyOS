/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:55
 * LastEditTime : 2022-06-17 16:28:13
 * Description  : 
 */

#include "systemconstant.h"
#include "proc.h"
#include "processmanager.h"
#include "paging.h"
#include "framemanager.h"
#include "swap.h"
#include "mmu.h"
#include "stackhandler.h"
#include "mem.h"
#include "systemcall.h"

ProcessSegment ProcessSegment::kernelDataSegment;
ProcessSegment ProcessSegment::kernelStackSegment;
ProcessSegment ProcessSegment::userDataSegment;
ProcessSegment ProcessSegment::userStackSegment;
ProcessSegment ProcessSegment::ESP0Segment;

ProcessSegment::ProcessSegment(uint32 start, uint32 end) : _startAddr(start), _endAddr(end) {}

bool ProcessSegment::includeAddr(uint32 addr) {
    return addr >= _startAddr && addr < _endAddr;
}

uint32 ProcessSegment::sizeOfPages() {
    return ((_endAddr - _startAddr) + 4095) >> 12;
}

uint32 ProcessSegment::sizeOfBytes() {
    return (_endAddr - _startAddr);
}

uint32 ProcessSegment::endAddr() {
    return _endAddr;
}

uint32 ProcessSegment::startAddr() {
    return _startAddr;
}

Vec<Page> ProcessSegment::toPages() {
    uint32 start = pageAlignLower(_startAddr);
    uint32 end = pageAlignUpper(_endAddr);
    Vec<Page> pages;
    Page newPage = Page(start);
    while(newPage.virtualAddr() < end) {
        pages.pushBack(newPage);
        newPage = newPage.higher();
    }
    return pages;
}

Vec<Page> ProcessSegment::toPages(int offset) {
    uint32 start;
    uint32 end;
    if(offset < 0) {
        end = pageAlignUpper(_endAddr);
        start = end + offset * PAGE_SIZE;
    }
    else {
        start = pageAlignLower(_startAddr);
        end = start + offset * PAGE_SIZE;
    }
    Vec<Page> pages;
    Page newPage = Page(start);
    while(newPage.virtualAddr() < end) {
        pages.pushBack(newPage);
        newPage = newPage.higher();
    }
    return pages;
}

Process::Process(
    ProcessPriviledge privilege,
    ProcessSegment dataSegment,
    Process* parent,
    uint32 ticks,
    uint32 startPoint
) {

///INITIALIZE-----------------------------
    _pid = 0;
    _privilege = privilege;
    _table = nullptr;

    _dataSegment = dataSegment;
    _stackSegment = privilege == ProcessPriviledge::USER ? ProcessSegment::userStackSegment : ProcessSegment::kernelStackSegment;
    _usedStackSegment = ProcessSegment(
        _stackSegment.endAddr() - PAGE_SIZE,
        _stackSegment.endAddr()  
    );

    _parent = parent;
    _children = Vec<Process*>();

    _ticks = ticks;
    _remainingTicks = ticks;

    _status = ProcessStatus::NEW;
///END OF INITIALIZE----------------------
    
///ALLOCATE-------------------------------
    
    // - Allocate space for Lev2 Table
    Frame* scndLevelTableFrame = FrameManager::allocateFrame(FrameFlag::LOCKED);
    
    // - Allocate space for User Stack
    Frame* initStackFrame = FrameManager::allocateFrame(FrameFlag::EMPTY);
    
    // - Allocate space for Data Segment
    Vec<Frame*> dataFrames = FrameManager::allocateFrames(_dataSegment.sizeOfPages(), FrameFlag::EMPTY);
    
///END OF ALLOCATE------------------------


///PREPARE--------------------------------
    
    // - Prepare Page Table
    PageTable* table = PageTable::fromPhysicalAddr(scndLevelTableFrame->physicalAddr());
    
    //    - Copy Kernel Page
    PageTable* kernelTable = ProcessManager::processOfPID(0)->pageTable();
    table->clone(kernelTable);

    //    - Set Stack Page
    Vec<Page> initStackPages = _stackSegment.toPages(-1);
    PageManager::mapPage(
        table,
        initStackPages[0],
        initStackFrame,
        PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
    );

    //    - Set Data Page
    Vec<Page> dataPages = _dataSegment.toPages();
    for(int i = 0; i < dataPages.size(); i++) {
        PageManager::mapPage(
            table,
            dataPages[i],
            dataFrames[i],
            PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
        );
    }

    // - Prepare init stack

    // Save area is for stack exchange macro in syscall handler when the process returns
    // =========================== <- stack top
    //     preserved for copy           ↑
    //   (faked interrupt frame)    5 * 4 bytes
    //   (faked eflags, cs, eip)        ↓
    //============================ <- save area bottom
    //      exit return val
    //        exit point
    //============================ <- esp points here (where the stack for process actually starts)
    //             ↓  process stack
    
    _state = ProcessState();
    _state._cr3 = toPhysicalAddress((uint32)table);
    _state._stack = _stackSegment.endAddr() - 20 - 12;
    _state._esp = _stackSegment.endAddr() - 8 - 12;
    _state._cs = privilege == ProcessPriviledge::KERNEL ? CODE_SELECTOR : USER_CODE_SELECTOR;
    _state._ss = privilege == ProcessPriviledge::KERNEL ? STACK_SELECTOR : USER_STACK_SELECTOR;
    _state._ds = privilege == ProcessPriviledge::KERNEL ? DATA_SELECTOR : USER_DATA_SELECTOR;
    _state._es = _state._ds;
    _state._fs = _state._ds;
    _state._gs = _state._ds;
    _state._eip = startPoint;
    _state._eflags = (1 << 9) | (1 << 1);

    // - Set exit point
    StackHandler stack = StackHandler(initStackFrame->virtualAddr() + PAGE_SIZE);
    for(int i = 0; i < 3; i++) {    //Saved area
        stack.push(0);
    }
    stack.push(0);  //syscall_exit ret val
    stack.push((uint32)syscall_exit);
    if(privilege == ProcessPriviledge::USER) {
        stack.push(USER_STACK_SELECTOR);
        stack.push(_state._esp);
        _state._stack -= 8;
    }

    // - Prepare table
    _table = table;

///END OF PREPARE-------------------------

}

Process* Process::_fork(
    Process* parentProcess
) {

    Process* childProcess = (Process*)malloc(sizeof(Process));

///INITIALIZE-----------------------------
    childProcess->_pid = 0;
    childProcess->_privilege = parentProcess->_privilege;
    childProcess->_table = nullptr;

    childProcess->_dataSegment = parentProcess->_dataSegment;
    //stack will be initialized later
    //esp0 will be initialized later

    childProcess->_parent = parentProcess;
    childProcess->_children = Vec<Process*>();

    childProcess->_ticks = parentProcess->_ticks;
    childProcess->_remainingTicks = parentProcess->_ticks;

    childProcess->_status = ProcessStatus::NEW;
///END OF INITIALIZE----------------------

///ALLOCATE-------------------------------

    // - Allocate space for Lev2 Table
    Frame* scndLevelTableFrame = FrameManager::allocateFrame(FrameFlag::LOCKED);

    // - Allocate space for private stack
    Vec<Frame*> initStackFrames = FrameManager::allocateFrames(
        parentProcess->_usedStackSegment.sizeOfPages(), 
        FrameFlag::EMPTY
    );

///END OF ALLOCATE------------------------

///PREPARE--------------------------------

    // // - Prepare Page Table
    // PageTable* table = PageTable::fromPhysicalAddr(scndLevelTableFrame->physicalAddr());
    
    // //    - Copy Parent Pages
    // PageTable* parentTable = parentProcess->_table;
    // table->clone(parentTable);
    PageTable* table = parentProcess->_table;

    //    - Find an empty entry
    int emptySlot = table->emptySlot();
    if(emptySlot == -1) {
        //Do something
    }

    //    - Set stack addr
    uint32 emptyAddrStart = emptySlot << 22;
    uint32 emptyAddrEnd = (emptySlot + 1) << 22;
    childProcess->_stackSegment = ProcessSegment(
        emptyAddrEnd - parentProcess->_stackSegment.sizeOfBytes(), 
        emptyAddrEnd
    );
    childProcess->_usedStackSegment = ProcessSegment(
        childProcess->_stackSegment.endAddr() - parentProcess->_usedStackSegment.sizeOfBytes(),
        childProcess->_stackSegment.endAddr()
    );

    //    - Set Stack Page
    Vec<Page> initStackPages = childProcess->_usedStackSegment.toPages();
    Vec<Page> parentStackPages = parentProcess->_usedStackSegment.toPages();
    for(int i = 0; i < initStackPages.size(); i++) {
        PageManager::mapPage(
            table,
            initStackPages[i],
            initStackFrames[i],
            PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
        );
        memcpy(
            (void*)initStackFrames[i]->virtualAddr(),
            (void*)parentStackPages[i].virtualAddr(),
            PAGE_SIZE
        );
    }

    // - Copy state
    childProcess->_state = parentProcess->_state;
    if(childProcess->_stackSegment.endAddr() > parentProcess->_stackSegment.endAddr()) {
        childProcess->_state._esp += childProcess->_stackSegment.endAddr() - parentProcess->_stackSegment.endAddr();
        if(childProcess->_privilege == ProcessPriviledge::KERNEL) {
            childProcess->_state._stack += childProcess->_stackSegment.endAddr() - parentProcess->_stackSegment.endAddr();
        }
    }
    else {
        childProcess->_state._esp -= parentProcess->_stackSegment.endAddr() - childProcess->_stackSegment.endAddr();
        if(childProcess->_privilege == ProcessPriviledge::KERNEL) {
            childProcess->_state._stack += childProcess->_stackSegment.endAddr() - parentProcess->_stackSegment.endAddr();
        }
    }
    childProcess->_state._cr3 = toPhysicalAddress((uint32)table);
    childProcess->_state._eax = 0;

    // - Prepare table
    childProcess->_table = table;

    return childProcess;

}

uint32 Process::pid() {
    return _pid;
}

PageTable* Process::pageTable() {
    return _table;
}

ProcessPriviledge Process::privilege() {
    return _privilege;
}

ProcessStatus Process::status() {
    return _status;
}

uint32 Process::remainingTicks() {
    return _remainingTicks;
}

bool Process::stackIncludeAddr(uint32 addr) {
    return _stackSegment.includeAddr(addr);
}

bool Process::dataIncludeAddr(uint32 addr) {
    return _dataSegment.includeAddr(addr);
}
void Process::tickOnce() {
    _remainingTicks = _remainingTicks == 0 ? 0 : _remainingTicks - 1;
}

void Process::resetTick() {
    _remainingTicks = _ticks;
}

void Process::addChild(Process* child) {
    _children.pushBack(child);
}

void Process::save(ProcessState* state) {
    _state = *state;
}

void Process::restore(ProcessState* state) {
    *state = _state;
}