/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 14:52:53
 * Description  : 
 */

#include "sched.h"

PCB* nodeToPCB(ListNode* node) {
    return (PCB*)((uint32)node - (uint32)&((PCB*)0)->readyTaskListNode);
}

List Scheduler::readyTaskList = List();
PCB* Scheduler::runningThread = nullptr;

PCB* allocPCB() {
    for(int i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(!PCBStatus[i]) {
            PCB* newPCB = (PCB*)&PCBBlock[i];
            newPCB->tID = i;
            PCBStatus[i] = true;
            return newPCB;
        }
    }
    return nullptr;
}

void freePCB(PCB* process) {
    PCBStatus[process->tID] = false;
}

void Scheduler::onTimeInterrupt() {
    if(runningThread == nullptr) {
        schedule();
    }
    else {
        runningThread->tickRemaining -= 1;
        if(runningThread->tickRemaining == 0) {
            schedule();
        }
    }
}

void Scheduler::schedule() {
    bool interruptStatus = InterruptManager::getInterruptStatus();
    asm_disable_interrupt();

    if(readyTaskList.isEmpty()) {
        return;
    }

    PCB* currentThread = nullptr;
    PCB* nextThread = nullptr;

    if(runningThread != nullptr && runningThread->status == JobStatus::RUNNING) {
        currentThread = runningThread;
        currentThread->tickRemaining = currentThread->priority * timeQuantum;
        readyTaskList.pushBack(&currentThread->readyTaskListNode);
    }
    else if(runningThread != nullptr && runningThread->status == JobStatus::DEAD) {
        currentThread = runningThread;
        freePCB(currentThread);
    }

    nextThread = nodeToPCB(readyTaskList.popFront());
    nextThread->status = JobStatus::RUNNING;
    runningThread = nextThread;
    asm_switch_thread((uint32)currentThread, (uint32)nextThread);

    InterruptManager::setInterruptStatus(interruptStatus);
}

void Scheduler::executeThread(void (*function)(), void* parameters, uint32 priority) {
    bool interruptStatus = InterruptManager::getInterruptStatus();
    asm_disable_interrupt();

    PCB* newThread = allocPCB();
    if(newThread == nullptr) {
        return;
    }

    newThread->stack = (uint32*)((int)newThread + PCB_MAX_SIZE);
    newThread->pID = 0;
    newThread->priority = priority;
    newThread->status = JobStatus::READY;
    newThread->tickRemaining = newThread->priority * timeQuantum;
    newThread->readyTaskListNode = ListNode();

    newThread->stack -= 7;
    newThread->stack[0] = 0;
    newThread->stack[1] = 0;
    newThread->stack[2] = 0;
    newThread->stack[3] = 0;
    newThread->stack[4] = (uint32)function;
    newThread->stack[5] = (uint32)threadExit;
    newThread->stack[6] = (uint32)parameters;

    readyTaskList.pushBack(&newThread->readyTaskListNode);

    InterruptManager::setInterruptStatus(interruptStatus);
}