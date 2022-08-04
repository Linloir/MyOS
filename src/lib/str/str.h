/*** 
 * Author       : Linloir
 * Date         : 2022-07-28 13:14:44
 * LastEditTime : 2022-07-28 13:40:48
 * Description  : Algorithm for strings
 */

#include "types.h"

#ifndef _STR_H_
#define _STR_H_

int strcmp(const char* str1, const char* str2);
int strncmp(const char* str1, const char* str2, uint32 cnt);
char* strcpy(char* dest, const char* src);
char* strcat(char* dest, const char* src, uint32 cnt);
char* strcat(char* dest, const char* src);
int strlen(const char* str);

#endif