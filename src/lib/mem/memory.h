/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 21:38:49
 * LastEditTime : 2022-07-24 21:43:22
 * Description  : 
 */

#ifndef _MEMORY_H_
#define _MEMORY_H_

extern "C" void* memcpy(void* dest, const void* src, unsigned int count);
extern "C" void* memset(void* mem, int value, unsigned int num);

#endif