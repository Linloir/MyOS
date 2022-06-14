/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:24:55
 * LastEditTime : 2022-06-14 10:18:31
 * Description  : 
 */

#include "os_constant.h"
#include "process.h"
#include "processmanager.h"
#include "paging.h"
#include "framemanager.h"
#include "swap.h"
#include "stackhandler.h"
#include "mmu.h"

ProcessSegment::ProcessSegment(uint32 start, uint32 end) : _startAddr(start), _endAddr(end) {}

ProcessSegment ProcessSegment::defaultKernelDataSegment() {
    return ProcessSegment(KERNEL_DATA_START, KERNEL_DATA_END);
}

ProcessSegment ProcessSegment::defaultKernelStackSegment() {
    return ProcessSegment(KERNEL_STACK_START, KERNEL_STACK_END);
}

ProcessSegment ProcessSegment::defaultUserDataSegment() {
    return ProcessSegment(0x0, 0x100000);
}

ProcessSegment ProcessSegment::defaultUserStackSegment() {
    return ProcessSegment(KERNEL_DATA_START - 0x200000, KERNEL_DATA_START - 0x100000);
}

ProcessSegment ProcessSegment::defaultESP0Segment() {
    return ProcessSegment(KERNEL_DATA_START - 0x100000, KERNEL_DATA_START);
}

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
    uint32 pid,
    ProcessPriviledge priviledge,

    ProcessSegment dataSegment,
    ProcessSegment stackSegment,
    
    Process* parent,

    uint32 ticks,

    uint32 entryPoint
) {

///INITIALIZE-----------------------------
    _pid = pid;
    _priviledge = priviledge;
    _table = nullptr;
    _esp = 0x0;

    _dataSegment = dataSegment;
    _stackSegment = stackSegment;
    _esp0Segment = priviledge == ProcessPriviledge::USER ? ProcessSegment::defaultESP0Segment() : ProcessSegment();
    
    _parent = parent;
    _children = Vec<Process*>();

    _ticks = ticks;
    _remainingTicks = ticks;

    _status = ProcessStatus::NEW;
///END OF INITIALIZE----------------------
    
///ALLOCATE-------------------------------
    
    // - Allocate space for Lev2 Table
    Frame* scndLevelTableFrame = FrameManager::allocateFrame(FrameFlag::LOCKED);
    
    // - Allocate space for ESP0
    Vec<Frame*> esp0Frames = FrameManager::allocateFrames(_esp0Segment.sizeOfPages(), FrameFlag::LOCKED);
    
    // - Allocate space for User Stack
    Frame* initStackFrame = FrameManager::allocateFrame(FrameFlag::EMPTY);
    
    // - Allocate space for Data Segment
    Vec<Frame*> dataFrames = FrameManager::allocateFrames(_dataSegment.sizeOfPages(), FrameFlag::EMPTY);
    
    // - Allocate new pid
    _pid = ProcessManager::allocPID();
    
///END OF ALLOCATE------------------------


///PREPARE--------------------------------
    
    // - Prepare Page Table
    PageTable* table = PageTable::fromPhysicalAddr(scndLevelTableFrame->physicalAddr());
    
    //    - Copy Kernel Page
    PageTable* kernelTable = ProcessManager::processOfPID(0)->pageTable();
    for(int i = 768; i < 1023; i++) {
        table[i] = kernelTable[i];
    }

    //    - Set ESP0 Stack Page
    Vec<Page> esp0Pages = _esp0Segment.toPages();
    for(int i = 0; i < esp0Pages.size(); i++) {
        PageManager::mapPage(
            table, 
            esp0Pages[i], 
            esp0Frames[i],
            PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
        );
    }

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
    StackHandler stack = StackHandler(initStackFrame->virtualAddr());
    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? STACK_SELECTOR : USER_STACK_SELECTOR));
    stack.push((uint32)_stackSegment.endAddr());

    stack.push((uint32)(1 << 9 | 1 << 1));  //eflags
    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? CODE_SELECTOR : USER_CODE_SELECTOR)); //cs
    stack.push((uint32)entryPoint); //eip

    stack.push((uint32)ProcessManager::_processStart);  //ret

    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? DATA_SELECTOR : USER_DATA_SELECTOR)); //ds
    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? DATA_SELECTOR : USER_DATA_SELECTOR)); //es
    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? DATA_SELECTOR : USER_DATA_SELECTOR)); //fs
    stack.push((uint32)(priviledge == ProcessPriviledge::KERNEL ? DATA_SELECTOR : USER_DATA_SELECTOR)); //gs

    stack.push((uint32)0x0);    //esi
    stack.push((uint32)0x0);    //edi
    stack.push((uint32)0x0);    //ebp

    stack.push((uint32)0x0);    //edx
    stack.push((uint32)0x0);    //ecx
    stack.push((uint32)0x0);    //ebx
    stack.push((uint32)0x0);    //eax

    // - Prepare esp
    _esp = stack._top;

    // - Prepare table
    _table = table;

///END OF PREPARE-------------------------

}

uint32 Process::pid() {
    return _pid;
}

PageTable* Process::pageTable() {
    return _table;
}

ProcessPriviledge Process::priviledge() {
    return _priviledge;
}

uint32 Process::esp() {
    return _esp;
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

bool Process::esp0IncludeAddr(uint32 addr) {
    return _esp0Segment.includeAddr(addr);
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

void Process::setEsp(uint32 esp) {
    _esp = esp;
}

void Process::setStatus(ProcessStatus status) {
    _status = status;
}