/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-06-13 22:57:23
 * Description  : Kernel Entry
 */
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "systemconstant.h"
#include "mmu.h"

extern "C" void kernel();

void initHeap(uint32 heapStartAddress);
void initScheduler();
void initInterrupt();

#endif