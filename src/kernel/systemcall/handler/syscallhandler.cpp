/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 21:11:21
 * LastEditTime : 2022-06-17 11:43:28
 * Description  : 
 */

#include "syscallhandler.h"
#include "systemio.h"
#include "processmanager.h"

void __syscall_panic__(ProcessState* state, uint32 code, uint32, uint32, uint32, uint32) {
    printf("[Panic] Kernel Panic with code %d\n", code);
    asm(
        ".hlt:\n\t"
        "hlt\n\t"
        "jmp .hlt\n\t"
    );
}

void __syscall_exit__(ProcessState* state, uint32 retval, uint32, uint32, uint32, uint32) {
    ProcessManager::exit(state, (int)retval);
}

void __syscall_wait__(ProcessState* state, uint32 retptr, uint32, uint32, uint32, uint32) {
    ProcessManager::wait(state, (int*)retptr);
}

void __syscall_sleep__(ProcessState* state, uint32 time, uint32, uint32, uint32, uint32) {
    while(time--) {
        asm(
            "sti\n\t"
            "hlt\n\t"
            "cli\n\t"
        );
    }
}

void __syscall_hibernate__(ProcessState* state, uint32, uint32, uint32, uint32, uint32) {
    ProcessManager::hibernate(state);
}

void __syscall_awake__(ProcessState* state, uint32 pid, uint32, uint32, uint32, uint32) {
    ProcessManager::awake(pid, state);
}

void __syscall_fork__(ProcessState* state, uint32, uint32, uint32, uint32, uint32) {
    ProcessManager::fork(state);
}

void __syscall_span__(ProcessState* state, uint32, uint32, uint32, uint32, uint32) {

}

void __syscall_port_write__(ProcessState* state, uint32 port, uint32 val, uint32, uint32, uint32) {
    uint16 _port = (uint16)port;
    byte _val = (byte)val;
    asm(
        "movw %[port], %%ax\n\t"
        "movw %%ax, %%dx\n\t"
        "movb %[val], %%al\n\t"
        "out %%al, %%dx\n\t"
        : 
        : [port]"r"(_port), [val]"r"(_val)
    );
}

void __syscall_port_read__(ProcessState* state, uint32 port, uint32, uint32, uint32, uint32) {
    uint16 _port = (uint16)port;
    byte _val = 0;
    asm(
        "movw %[port], %%dx\n\t"
        "in %%dx, %[val]\n\t"
        : [val]"=r"(_val)
        : [port]"r"(_port)  
    );
    state->_eax = (uint32)_val;
}

void __syscall_pid__(ProcessState* state, uint32, uint32, uint32, uint32, uint32) {
    state->_eax = ProcessManager::current()->pid();
}