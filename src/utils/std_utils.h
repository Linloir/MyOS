/*** 
 * Author       : Linloir
 * Date         : 2022-05-17 20:29:39
 * LastEditTime : 2022-05-17 17:03:55
 * Description  : Temporary standard library
 */

#ifndef _STD_UTILS_H_
#define _STD_UTILS_H_

#include "os_type.h"

#define INT_SIZE_OF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap, v) (ap = (va_list)&v + INT_SIZE_OF(v))
#define va_arg(ap, type) (*(type*)((ap += INT_SIZE_OF(type)) - INT_SIZE_OF(type)))
#define va_end(ap) (ap = (va_list)0)

typedef char* va_list;

template<typename T>
void swap(T &x, T &y);

int printf_add_to_buffer(char* buffer, char c, int &idx, const int BUF_LEN);
void itos(char* numStr, uint32 num, uint32 mod);
int printf(const char* format, ...);

#endif