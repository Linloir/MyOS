/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:21
 * LastEditTime : 2022-06-17 00:15:43
 * Description  : 
 */
/***
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:21
 * LastEditTime : 2022-06-14 23:52:57
 * Description  :
 */

#include "syscallmanager.h"

Syscall SyscallManager::_syscall[MAX_SYSTEM_CALL];

void SyscallManager::handler(ProcessState* state, uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4) {
    _syscall[vec](state, p0, p1, p2, p3, p4);
} 

void SyscallManager::initialize() {
    for(int i = 0; i < MAX_SYSTEM_CALL; i++) {
        _syscall[i] = (Syscall)0;
    }
    _syscall[0] = testSyscall;
    _syscall[1] = __syscall_fork__;
}

