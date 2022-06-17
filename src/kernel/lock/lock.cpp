/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 16:58:13
 * LastEditTime : 2022-06-17 12:05:19
 * Description  : 
 */

#include "lock.h"
#include "systemcall.h"

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
    awaitList = Vec<uint32>();
}

SemLock::SemLock(int permits) {
    availablePermits = permits;
    permitLock = SpinLock();
    awaitList = Vec<uint32>();
}

void SemLock::acquire() {
    permitLock.lock();
    if(availablePermits == 0) {
        permitLock.release();
        awaitList.pushBack(syscall_pid());
        syscall_hibernate();
        permitLock.lock();
    }
    availablePermits--;
    permitLock.release();
}

void SemLock::release() {
    permitLock.lock();
    availablePermits++;
    if(!awaitList.isEmpty()) {
        uint32 pid = awaitList.front();
        awaitList.erase(0);
        permitLock.release();
        syscall_awake(pid);
    }
    else {
        permitLock.release();
    }
}