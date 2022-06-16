/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 20:29:47
 * LastEditTime : 2022-06-16 10:54:43
 * Description  : 
 */

#include "os_constant.h"
#include "processmanager.h"
#include "paging.h"
#include "interrupt.h"
#include "std_utils.h"
#include "tss.h"

BitMap ProcessManager::_pids;
Vec<Process*> ProcessManager::_readyProcesses;
Vec<Process*> ProcessManager::_allProcesses;
Vec<Process*> ProcessManager::_awaitProcesses;
Process* ProcessManager::_curProcess;

void ProcessManager::_processStart(InterruptFrame* f) {
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

    if(cur->_status == ProcessStatus::RUNNING) {
        _readyProcesses.pushBack(cur);
    }
    _readyProcesses.erase(0);
    cur->setStatus(ProcessStatus::READY);
    next->setStatus(ProcessStatus::RUNNING);
    _curProcess = next;

    _switchProcess(cur, next);
    TSSManager::setESP0(next->esp0());

    setInterruptStatus(interrupt);
}

void ProcessManager::_switchProcess(Process* cur, Process* next) {
    asm_switch_process(&cur->_esp, next->_table->physicalAddr(), next->_esp);
}

void ProcessManager::_hasenAwakeProcess(Process* process) {
    _awaitProcesses.erase(process);
    _awaitProcesses.insert(0, process);
}

void ProcessManager::initialize() {
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
    process->_pid = allocPID();
    _allProcesses.pushBack(process);
    _readyProcesses.pushBack(process);
    setInterruptStatus(interrupt);
}

void ProcessManager::haltProcess(Process* process) {
    bool interrupt = getInterruptStatus();
    setInterruptStatus(false);
    if(process != _curProcess) {
        _readyProcesses.erase(process);
        process->_status = ProcessStatus::BLOCKED;
        _awaitProcesses.pushBack(process);
        return;
    }

    Process* cur = _curProcess;
    Process* next = _readyProcesses.front();

    _readyProcesses.erase(0);
    cur->_status = ProcessStatus::BLOCKED;
    next->_status = ProcessStatus::RUNNING;
    _awaitProcesses.pushBack(cur);
    _curProcess = next;

    _switchProcess(cur, next);

    setInterruptStatus(interrupt);
}

void ProcessManager::awakeProcess(Process* process, AwakeMethod method) {
    if(method == AwakeMethod::HASEN) {
        _hasenAwakeProcess(process);
    }
}

void ProcessManager::forkProcess(Process* process) {
    Process* parent = process;
    Process* child = (Process*)malloc(sizeof(Process));
    *child = Process::inheritFrom(parent);
    parent->addChild(child);
}