/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:47
 * LastEditTime : 2022-06-17 11:09:54
 * Description  : 
 */

#include "systemconstant.h"
#include "processmanager.h"
#include "paging.h"
#include "interrupt.h"
#include "systemio.h"
#include "tss.h"

BitMap ProcessManager::_pids;
Vec<Process*> ProcessManager::_readyProcesses;
Vec<Process*> ProcessManager::_allProcesses;
Vec<Process*> ProcessManager::_awaitProcesses;
Process* ProcessManager::_curProcess;

void ProcessManager::_processExit() {
    
}

void ProcessManager::_schedule(ProcessState* state) {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);

    printf("In schedule\n");
    
    if(_readyProcesses.isEmpty()) {
        return;
    }

    Process* cur = _curProcess;
    Process* next = _readyProcesses.front();

    if(cur->_status == ProcessStatus::RUNNING) {
        _readyProcesses.pushBack(cur);
    }
    if(cur->_status == ProcessStatus::BLOCKED) {
        _awaitProcesses.pushBack(cur);
    }
    _readyProcesses.erase(0);

    cur->_status = ProcessStatus::READY;
    next->_status = ProcessStatus::RUNNING;

    _curProcess = next;


    cur->save(state);
    next->restore(state);

    setInterruptStatus(interrupt);
}

void ProcessManager::initialize() {
    //Initialize Process Segment
    ProcessSegment::kernelDataSegment = ProcessSegment(0x0, 0x0);
    ProcessSegment::kernelStackSegment = ProcessSegment(
        KERNEL_DATA_START - DEFAULT_ESP0_STACK_SIZE - DEFAULT_STACK_SIZE, 
        KERNEL_DATA_START - DEFAULT_ESP0_STACK_SIZE
    );
    ProcessSegment::userDataSegment = ProcessSegment(0x0, 0x100000);
    ProcessSegment::userStackSegment = ProcessSegment(
        KERNEL_DATA_START - DEFAULT_ESP0_STACK_SIZE - DEFAULT_STACK_SIZE, 
        KERNEL_DATA_START - DEFAULT_ESP0_STACK_SIZE
    );
    ProcessSegment::ESP0Segment = ProcessSegment(
        KERNEL_DATA_START - DEFAULT_ESP0_STACK_SIZE, 
        KERNEL_DATA_START
    );

    _pids = BitMap(25536);
    _allProcesses = Vec<Process*>();
    _readyProcesses = Vec<Process*>();
    _awaitProcesses = Vec<Process*>();
    _curProcess = nullptr;

    _pids.set(0, true);
    
    Process* kernelProcess = (Process*)malloc(sizeof(Process));
    *kernelProcess = Process();
    
    kernelProcess->_pid = 0;
    kernelProcess->_priviledge = ProcessPriviledge::KERNEL;
    kernelProcess->_table = PageTable::fromPhysicalAddr(getCR3());
    kernelProcess->_dataSegment = ProcessSegment(0xC0000000, 0xC0100000);
    kernelProcess->_stackSegment = ProcessSegment(0xFFE00000, 0xFFFFFFFC);
    kernelProcess->_parent = nullptr;
    kernelProcess->_children = Vec<Process*>();
    kernelProcess->_ticks = 30;
    kernelProcess->_remainingTicks = 30;
    kernelProcess->_status = ProcessStatus::RUNNING;

    _curProcess = kernelProcess;
    _allProcesses.pushBack(kernelProcess);

    printf("Process Manager Initialized!\n");
}

void ProcessManager::schedule(ProcessState* state) {
    if(_curProcess->remainingTicks() > 0) {
        _curProcess->tickOnce();
        return;
    }
    else {
        _curProcess->resetTick();
        _schedule(state);
    }
}

Process* ProcessManager::current() {
    return _curProcess;
}

Process* ProcessManager::processOfPID(uint32 pid) {
    for(int i = 0; i < _allProcesses.size(); i++) {
        if(_allProcesses[i]->pid() != pid) {
            continue;
        }
        return _allProcesses[i];
    }
    return nullptr;
}

void ProcessManager::execute(Process* process) {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);
    process->_status = ProcessStatus::READY;
    process->_pid = _pids.alloc();
    _allProcesses.pushBack(process);
    _readyProcesses.pushBack(process);
    setInterruptStatus(interrupt);
}

void ProcessManager::hibernate(ProcessState* state) {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);

    _curProcess->_status = ProcessStatus::BLOCKED;
    _schedule(state);

    setInterruptStatus(interrupt);
}

void ProcessManager::awake(uint32 pid, ProcessState* state) {
    Process* proc = processOfPID(pid);
    proc->_status = ProcessStatus::READY;
    _readyProcesses.insert(0, proc);
    _schedule(state);
}

void ProcessManager::fork(ProcessState* state) {
    Process* parent = _curProcess;
    parent->save(state);
    Process* child = Process::_fork(parent);
    child->_pid = _pids.alloc();
    parent->_state._eax = child->_pid;
    child->_status = ProcessStatus::RUNNING;
    parent->_status = ProcessStatus::READY;
    _readyProcesses.pushBack(parent);
    _curProcess = child;
    child->restore(state);
}

void ProcessManager::exit(ProcessState* state, int retval) {
    while(true){}
}

void ProcessManager::wait(ProcessState* state, int* retptr) {
    while(true){}
}