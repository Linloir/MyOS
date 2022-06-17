/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 20:33:49
 * LastEditTime : 2022-06-17 16:02:23
 * Description  : system calls
 */

#ifndef _SYSTEM_CALL_H_
#define _SYSTEM_CALL_H_

#include "systemtype.h"

#define Naked __attribute__((naked))

Naked uint32 syscall(uint32 vec);
Naked uint32 syscall(uint32 vec, uint32 p0);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);

void syscall_panic(uint32 code);
void syscall_sleep(uint32 time);
void syscall_exit(uint32 retval);
uint32 syscall_wait(int* retptr);
void syscall_hibernate();
void syscall_awake(uint32 pid);
uint32 syscall_fork();
uint32 syscall_span();
void syscall_portWrite(uint32 port, uint32 val);
uint32 syscall_portRead(uint32 port);
uint32 syscall_pid();

#endif
