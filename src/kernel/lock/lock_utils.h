/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 19:33:09
 * LastEditTime : 2022-05-22 16:17:25
 * Description  : Lock utils
 */


#ifndef _LOCK_UTILS_H_
#define _LOCK_UTILS_H_

#include "os_type.h"

extern "C" void asm_atomic_exchange(uint32* keyA, uint32* keyB);
extern "C" uint32 asm_atomic_test(uint32* key);

#endif