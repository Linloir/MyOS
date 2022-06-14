/*** 
 * Author       : Linloir
 * Date         : 2022-06-14 19:51:13
 * LastEditTime : 2022-06-14 19:51:14
 * Description  : 
 */

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