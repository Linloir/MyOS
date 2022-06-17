/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:21
 * LastEditTime : 2022-06-17 11:49:03
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
        _syscall[i] = __syscall_panic__;
    }
    _syscall[0x0] = __syscall_panic__;
    _syscall[0x1] = __syscall_sleep__;
    _syscall[0x2] = __syscall_exit__;
    _syscall[0x3] = __syscall_wait__;
    _syscall[0x4] = __syscall_hibernate__;
    _syscall[0x5] = __syscall_awake__;
    _syscall[0x6] = __syscall_fork__;
    _syscall[0x7] = __syscall_span__;
    _syscall[0x8] = __syscall_port_write__;
    _syscall[0x9] = __syscall_port_read__;
    _syscall[0xA] = __syscall_pid__;
}

