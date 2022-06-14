/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 19:51:51
 * LastEditTime : 2022-06-14 19:52:16
 * Description  : 
 */
#ifndef _MISCELLANEOUS_H_
#define _MISCELLANEOUS_H_

extern "C" int __cxa_atexit(void *object, void (*destructor)(void *), void *dso_handle);
extern void* __dso_handle;

extern "C" void* memcpy(void* dest, const void* src, unsigned int count);
extern "C" void* memset(void* mem, int value, unsigned int num);

#endif