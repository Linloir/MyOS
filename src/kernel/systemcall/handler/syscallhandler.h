/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 21:10:12
 * LastEditTime : 2022-06-17 00:15:38
 * Description  : 
 */

#ifndef _SYSCALL_HANDLER_H_
#define _SYSCALL_HANDLER_H_

#include "os_type.h"
#include "proc.h"

void testSyscall(ProcessState* state, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);
void __syscall_fork__(ProcessState* state, uint32, uint32, uint32, uint32, uint32);

#endif
