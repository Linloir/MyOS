/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 20:33:49
 * LastEditTime : 2022-06-16 22:20:56
 * Description  : system calls
 */

#ifndef _SYSTEM_CALL_H_
#define _SYSTEM_CALL_H_

#include "os_type.h"

#define Naked __attribute__((naked))

Naked uint32 syscall(uint32 vec);
Naked uint32 syscall(uint32 vec, uint32 p0);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3);
Naked uint32 syscall(uint32 vec, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);

uint32 exit(uint32 retval);
uint32 sleep();
uint32 awake(uint32 pid);
uint32 sysTest(uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);
uint32 fork();

#endif
