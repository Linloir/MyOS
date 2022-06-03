/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-06-03 15:03:26
 * Description  : Kernel Entry
 */
#ifndef _KERNEL_H_
#define _KERNEL_H_

#include "os_constant.h"

extern "C" void kernel();

void initHeap();

#endif