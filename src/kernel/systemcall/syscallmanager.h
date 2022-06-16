/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:12
 * LastEditTime : 2022-06-16 22:05:03
 * Description  : 
 */

#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_

#include "os_constant.h"
#include "os_type.h"
#include "syscallhandler.h"

using Syscall = uint32 (*)(ProcessState*, uint32, uint32, uint32, uint32, uint32);

class SyscallManager {
    private:
        static Syscall _syscall[MAX_SYSTEM_CALL];
    public:
        static void initialize();
        static uint32 handler(ProcessState* state, uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);
};

#endif