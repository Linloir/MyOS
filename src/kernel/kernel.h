/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-06-04 22:43:43
 * Description  : Kernel Entry
 */
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "os_constant.h"
#include "mmu.h"

extern "C" void kernel();

void initHeap(uint32 heapStartAddress);
void initFrames(int totalFrames, int mappedFrames);
void initGlobalDescriptorTable();
void initPaging();
void initScheduler();
void initInterrupt();

#endif