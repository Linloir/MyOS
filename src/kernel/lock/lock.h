/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 16:58:20
 * LastEditTime : 2022-06-14 14:46:36
 * Description  : Spin lock and 
 */

#ifndef _LOCK_H_
#define _LOCK_H_

#include "os_type.h"
#include "vector.h"

class Process;

class SpinLock {
    private:
        uint32 status;
    public:
        SpinLock();
        void lock();    //Try lock the spin lock, blocking
        bool tryLock(); //Try lock the spin lock, non-blocking
        void release(); //Release lock, non-blocking
};

class SemLock {
    private:
        int availablePermits = 0;
        SpinLock permitLock;
        Vec<Process*> awaitList;
    public:
        SemLock();
        SemLock(int permits);
        int permits() {return availablePermits;}
        void acquire();     //Try acquire permission, blocking
        void release();     //Release permission, non-blocking
};

extern "C" void asm_atomic_exchange(uint32* keyA, uint32* keyB);
extern "C" uint32 asm_atomic_test(uint32* key);

#endif