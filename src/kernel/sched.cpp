/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 20:28:10
 * Description  : 
 */

#include "sched.h"
#include "sched_utils.h"
#include "interrupt.h"

/* Simple malloc here, will be delete afterwards */
char PCBBlock[PCB_MAX_SIZE * MAX_PROCESS_COUNT];
bool PCBStatus[MAX_PROCESS_COUNT];

PCB* nodeToPCB(ListNode* node) {
    return (PCB*)((uint32)node - (uint32)&((PCB*)0)->scheduleListNode);
}

PCB* allocPCB() {
    for(int i = 0; i < MAX_PROCESS_COUNT; i++) {
        if(!PCBStatus[i]) {
            PCB* newPCB = (PCB*)&PCBBlock[i * PCB_MAX_SIZE];
            newPCB->tID = i;
            PCBStatus[i] = true;
            return newPCB;
        }
    }
    return 0;
}

void freePCB(PCB* process) {
    PCBStatus[process->tID] = false;
}

PCB* Scheduler::runningThread;
List Scheduler::readyTaskList;

void Scheduler::initialize() {
    runningThread = 0;
    readyTaskList = List();
    for(int i = 0; i < MAX_PROCESS_COUNT; i++) {
        PCBStatus[i] = false;
    }
}

void Scheduler::onTimeInterrupt() {
    if(runningThread == 0) {
        schedule();
    }
    else {
        runningThread->tickRemaining -= 1;
        if(runningThread->tickRemaining == 0) {
            schedule();
        }
    }
}

PCB* Scheduler::currentRunningThread() {
    return runningThread;
}

void Scheduler::schedule() {
    bool interruptStatus = InterruptManager::getInterruptStatus();
    asm_disable_interrupt();

    if(readyTaskList.isEmpty()) {
        return;
    }

    PCB* currentThread = runningThread;
    PCB* nextThread = nodeToPCB(readyTaskList.popFront());
    
    if(runningThread != 0) {
        if(runningThread->status == JobStatus::RUNNING) {
            if(runningThread->tickRemaining > 0) {
                readyTaskList.pushFront(&currentThread->scheduleListNode);
            }
            else {
                currentThread->tickRemaining = currentThread->priority * timeQuantum;
                currentThread->status = JobStatus::READY;
                readyTaskList.pushBack(&currentThread->scheduleListNode);
            }
        }
        else if(runningThread->status == JobStatus::DEAD) {
            freePCB(currentThread);
        }
    }

    nextThread->status = JobStatus::RUNNING;
    runningThread = nextThread;
    asm_switch_thread((uint32)currentThread, (uint32)nextThread);

    InterruptManager::setInterruptStatus(interruptStatus);
}

void Scheduler::executeThread(void (*function)(), void* parameters, uint32 priority) {
    bool interruptStatus = InterruptManager::getInterruptStatus();
    asm_disable_interrupt();

    PCB* newThread = allocPCB();
    if(newThread == 0) {
        return;
    }

    newThread->stack = (uint32*)((int)newThread + PCB_MAX_SIZE);
    newThread->pID = 0;
    newThread->priority = priority;
    newThread->status = JobStatus::READY;
    newThread->tickRemaining = newThread->priority * timeQuantum;
    newThread->scheduleListNode = ListNode();

    newThread->stack -= 7;
    newThread->stack[0] = 0;
    newThread->stack[1] = 0;
    newThread->stack[2] = 0;
    newThread->stack[3] = 0;
    newThread->stack[4] = (uint32)function;
    newThread->stack[5] = (uint32)threadExit;
    newThread->stack[6] = (uint32)parameters;

    readyTaskList.pushBack(&newThread->scheduleListNode);

    InterruptManager::setInterruptStatus(interruptStatus);
}

void Scheduler::awakeThreadMESA(PCB* thread) {
    readyTaskList.pushBack(&thread->scheduleListNode);
}

void Scheduler::awakeThreadHasen(PCB* thread) {
    readyTaskList.pushFront(&thread->scheduleListNode);
}

void Scheduler::awakeThreadHoare(PCB* thread) {
    readyTaskList.pushFront(&thread->scheduleListNode);
    schedule();
}

void Scheduler::threadExit() {
    runningThread->status = JobStatus::DEAD;
    schedule();
}