/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 16:58:13
 * LastEditTime : 2022-06-08 11:11:05
 * Description  : 
 */

#include "lock.h"
#include "lock_utils.h"
#include "proc.h"
#include "std_utils.h"

SpinLock::SpinLock() {
    status = 0;
}

void SpinLock::lock() {
    while(!tryLock()){}
}

bool SpinLock::tryLock() {
    uint32 newStatus = 1;
    // asm_atomic_exchange(&newStatus, &status);
    newStatus = asm_atomic_test(&status);
    return newStatus == 0;
}

void SpinLock::release() {
    status = 0;
}

SemLock::SemLock() {
    availablePermits = 1;
    permitLock = SpinLock();
    awaitList = List();
}

SemLock::SemLock(int permits) {
    availablePermits = permits;
    permitLock = SpinLock();
    awaitList = List();
}

void SemLock::acquire() {
    permitLock.lock();
    if(availablePermits == 0) {
        PCB* currentThread = Scheduler::currentRunningThread();
        currentThread->status = JobStatus::BLOCKED;
        awaitList.pushBack(&currentThread->scheduleListNode);
        permitLock.release();
        Scheduler::schedule();
        permitLock.lock();
    }
    availablePermits--;
    permitLock.release();
}

void SemLock::release() {
    permitLock.lock();
    availablePermits++;
    if(!awaitList.isEmpty()) {
        PCB* thread = nodeToPCB(awaitList.popFront());
        thread->status = JobStatus::READY;
        permitLock.release();
        Scheduler::awakeThreadHoare(thread);
    }
    else {
        permitLock.release();
    }
}