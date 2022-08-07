/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 11:10:35
 * LastEditTime : 2022-08-07 22:13:07
 * Description  : Kernel
 */

#include "types.h"
#include "bootinfo.h"

#ifndef _KERNEL_H_
#define _KERNEL_H_

extern "C" void kernel(BootInfo& bootInfo);

#endif
