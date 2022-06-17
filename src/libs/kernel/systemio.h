/*** 
 * Author       : Linloir
 * Date         : 2022-06-17 10:08:08
 * LastEditTime : 2022-06-17 11:51:44
 * Description  : 
 */

#ifndef _SYSTEM_IO_H_
#define _SYETEM_IO_H_

#include "systemtype.h"
#include "algorithm.h"

#define INT_SIZE_OF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))
#define va_start(ap, v) (ap = (va_list)&v + INT_SIZE_OF(v))
#define va_arg(ap, type) (*(type*)((ap += INT_SIZE_OF(type)) - INT_SIZE_OF(type)))
#define va_end(ap) (ap = (va_list)0)

typedef char* va_list;

int printf_add_to_buffer(char* buffer, char c, int &idx, const int BUF_LEN);
int printf(const char* format, ...);

#endif