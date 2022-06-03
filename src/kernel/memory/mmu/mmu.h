/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 14:03:21
 * LastEditTime : 2022-06-03 14:05:22
 * Description  : Transform address
 */

#ifndef _MMU_H_
#define _MMU_H_

#include "os_type.h"

#define OFFSET 0xB0100000

uint32 physicToVirtual(uint32);
uint32 VirtualToPhysic(uint32);

#endif