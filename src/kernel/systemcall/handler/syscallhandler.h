/*** 
 * Author       : Linloir
 * Date         : 2022-06-16 21:10:12
 * LastEditTime : 2022-06-16 22:04:47
 * Description  : 
 */

#ifndef _SYSCALL_HANDLER_H_
#define _SYSCALL_HANDLER_H_

#include "os_type.h"
#include "proc.h"

uint32 testSyscall(ProcessState* state, uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);

#endif
