/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:47
 * LastEditTime : 2022-06-14 10:25:53
 * Description  : 
 */

#include "os_constant.h"
#include "processmanager.h"
#include "paging.h"
#include "interrupt.h"
#include "std_utils.h"

BitMap ProcessManager::_pids;
Vec<Process*> ProcessManager::_readyProcesses;
Vec<Process*> ProcessManager::_allProcesses;
Process* ProcessManager::_curProcess;

__attribute__((interrupt)) void ProcessManager::_processStart(void* _) {
    asm("sti");
}

void ProcessManager::_processExit() {
    _curProcess->setStatus(ProcessStatus::DEAD);
    _schedule();
}

void ProcessManager::_schedule() {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);

    printf("In schedule\n");
    
    if(_readyProcesses.isEmpty()) {
        return;
    }

    Process* cur = _curProcess;
    Process* next = _readyProcesses.front();

    _readyProcesses.erase(0);
    cur->setStatus(ProcessStatus::READY);
    next->setStatus(ProcessStatus::RUNNING);
    _curProcess = next;
    _readyProcesses.pushBack(cur);

    _switchProcess(cur, next);

    setInterruptStatus(interrupt);
}

void ProcessManager::_switchProcess(Process* cur, Process* next) {
    //Preserve old process
    asm volatile(
        "push %%ds\n\t"
        "push %%es\n\t"
        "push %%fs\n\t"
        "push %%gs\n\t"
        "push %%esi\n\t"
        "push %%edi\n\t"
        "push %%ebp\n\t"
        "push %%edx\n\t"
        "push %%ecx\n\t"
        "push %%ebx\n\t"
        "push %%eax\n\t"
        "movl %%esp, %0"
        : "=r"(_curProcess->_esp)
    );
    //Move to new esp
    setCR3((uint32)next->_table);
    asm volatile(
        "movl %[esp], %%esp\n\t"
        "pop %%eax\n\t"
        "pop %%ebx\n\t"
        "pop %%ecx\n\t"
        "pop %%edx\n\t"
        "pop %%ebp\n\t"
        "pop %%edi\n\t"
        "pop %%edi\n\t"
        "pop %%esi\n\t"
        "pop %%gs\n\t"
        "pop %%fs\n\t"
        "pop %%es\n\t"
        "pop %%ds"
        : 
        : [esp]"r"(next->_esp)
    );
}

void ProcessManager::initialize() {
    _pids = BitMap(25536);
    _allProcesses = Vec<Process*>();
    _readyProcesses = Vec<Process*>();
    _curProcess = nullptr;

    _pids.set(0, true);
    
    Process* kernelProcess = (Process*)malloc(sizeof(Process));
    *kernelProcess = Process();
    
    kernelProcess->_pid = 0;
    kernelProcess->_priviledge = ProcessPriviledge::KERNEL;
    kernelProcess->_table = PageTable::fromPhysicalAddr(getCR3());
    kernelProcess->_dataSegment = ProcessSegment::defaultKernelDataSegment();
    kernelProcess->_stackSegment = ProcessSegment::defaultKernelStackSegment();
    kernelProcess->_esp0Segment = ProcessSegment();
    kernelProcess->_parent = nullptr;
    kernelProcess->_children = Vec<Process*>();
    kernelProcess->_ticks = 30;
    kernelProcess->_remainingTicks = 30;
    kernelProcess->_status = ProcessStatus::RUNNING;

    _curProcess = kernelProcess;
    _allProcesses.pushBack(kernelProcess);

    printf("Process Manager Initialized!\n");
}

void ProcessManager::onTimeTick() {
    if(_curProcess->remainingTicks() > 0) {
        _curProcess->tickOnce();
        return;
    }
    else {
        _curProcess->resetTick();
        _schedule();
    }
}

Process* ProcessManager::curProcess() {
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

uint32 ProcessManager::allocPID() {
    return _pids.alloc();
}

void ProcessManager::executeProcess(Process* process) {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);
    process->setStatus(ProcessStatus::READY);
    _allProcesses.pushBack(process);
    _readyProcesses.pushBack(process);
    setInterruptStatus(interrupt);
}