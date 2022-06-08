/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 16:58:20
 * LastEditTime : 2022-06-08 11:10:57
 * Description  : Spin lock and 
 */

#ifndef _LOCK_H_
#define _LOCK_H_

#include "os_type.h"
#include "structure.h"

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
    List awaitList;
public:
    SemLock();
    SemLock(int permits);
    int permits() {return availablePermits;}
    void acquire();     //Try acquire permission, blocking
    void release();     //Release permission, non-blocking
};

#endif