/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:12
 * LastEditTime : 2022-06-17 00:14:30
 * Description  : 
 */

#ifndef _SYSCALL_MANAGER_H_
#define _SYSCALL_MANAGER_H_

#include "systemconstant.h"
#include "systemtype.h"
#include "syscallhandler.h"

using Syscall = void (*)(ProcessState*, uint32, uint32, uint32, uint32, uint32);

class SyscallManager {
    private:
        static Syscall _syscall[MAX_SYSTEM_CALL];
    public:
        static void initialize();
        static void handler(ProcessState* state, uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);
};

#endif