/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:12
 * LastEditTime : 2022-06-15 10:08:31
 * Description  : 
 */

#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_

#include "os_constant.h"
#include "systemservices.h"

#define Naked __attribute__((naked))

class SyscallManager {
    friend void syscallInterruptHandler();

    private:
        static Syscall _syscall[MAX_SYSTEM_CALL];
        static uint32 _handler(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);
    public:
        static void initialize();
};

Naked uint32 syscall(uint32 vec);
Naked uint32 syscall(uint32 vec, uint32 p0);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);

#endif