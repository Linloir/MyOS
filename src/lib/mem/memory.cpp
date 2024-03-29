/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 21:38:53
 * LastEditTime : 2022-07-24 21:42:46
 * Description  : 
 */

#include "memory.h"

extern "C" void* memcpy(void* dest, const void* src, unsigned int count) {
  char* dst8 = (char*)dest;
  char* src8 = (char*)src;

  while (count--) {
    *dst8++ = *src8++;
  }
  return dest;
}

extern "C" void* memset(void* mem, int value, unsigned int num) {
  char* m = (char*)mem;
  char v = value;
  while (num-- != 0)
    *(m + num) = v;
  return mem;
}