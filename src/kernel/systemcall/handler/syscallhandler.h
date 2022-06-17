/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 21:10:12
 * LastEditTime : 2022-06-17 11:19:52
 * Description  : 
 */

#ifndef _SYSCALL_HANDLER_H_
#define _SYSCALL_HANDLER_H_

#include "systemtype.h"
#include "proc.h"

void __syscall_panic__(ProcessState* state, uint32 code, uint32, uint32, uint32, uint32);
void __syscall_sleep__(ProcessState* state, uint32 time, uint32, uint32, uint32, uint32);
void __syscall_exit__(ProcessState* state, uint32 retval, uint32, uint32, uint32, uint32);
void __syscall_wait__(ProcessState* state, uint32 retptr, uint32, uint32, uint32, uint32);
void __syscall_hibernate__(ProcessState* state, uint32, uint32, uint32, uint32, uint32);
void __syscall_awake__(ProcessState* state, uint32 pid, uint32, uint32, uint32, uint32);
void __syscall_fork__(ProcessState* state, uint32, uint32, uint32, uint32, uint32);
void __syscall_span__(ProcessState* state, uint32, uint32, uint32, uint32, uint32);
void __syscall_port_write__(ProcessState* state, uint32 port, uint32 val, uint32, uint32, uint32);
void __syscall_port_read__(ProcessState* state, uint32 port, uint32, uint32, uint32, uint32);
void __syscall_pid__(ProcessState* state, uint32, uint32, uint32, uint32, uint32);

#endif
