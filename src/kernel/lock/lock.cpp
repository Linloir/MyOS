/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 16:58:13
 * LastEditTime : 2022-06-14 14:42:59
 * Description  : 
 */

#include "lock.h"
#include "std_utils.h"
#include "processmanager.h"

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
    awaitList = Vec<Process*>();
}

SemLock::SemLock(int permits) {
    availablePermits = permits;
    permitLock = SpinLock();
    awaitList = Vec<Process*>();
}

void SemLock::acquire() {
    permitLock.lock();
    if(availablePermits == 0) {
        Process* proc = ProcessManager::curProcess();
        permitLock.release();
        ProcessManager::haltProcess(proc);
        permitLock.lock();
    }
    availablePermits--;
    permitLock.release();
}

void SemLock::release() {
    permitLock.lock();
    availablePermits++;
    if(!awaitList.isEmpty()) {
        Process* proc = awaitList.front();
        awaitList.erase(0);
        permitLock.release();
        ProcessManager::awakeProcess(proc, ProcessManager::AwakeMethod::DEFAULT);
    }
    else {
        permitLock.release();
    }
}