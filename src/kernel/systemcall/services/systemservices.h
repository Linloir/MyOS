/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 21:41:48
 * LastEditTime : 2022-06-15 10:02:15
 * Description  : 
 */

#ifndef _SYSTEM_SERVICES_H_
#define _SYSTEM_SERVICES_H_

#include "os_type.h"
#include "std_utils.h"

using Syscall = uint32 (*)(uint32, uint32, uint32, uint32, uint32);

uint32 syscallTest(uint32 p0, uint32 p1, uint32 p2, uint32 p3, uint32 p4);

#endif