/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 10:08:53
 * LastEditTime : 2022-06-17 11:34:47
 * Description  : 
 */

#ifndef _MEM_H_
#define _MEM_H_

extern "C" void* memcpy(void* dest, const void* src, unsigned int count);
extern "C" void* memset(void* mem, int value, unsigned int num);

#endif