/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 10:25:10
 * LastEditTime : 2022-06-13 14:23:29
 * Description  : 
 */

extern "C" int __cxa_atexit( 
    void *object, 
    void (*destructor)(void *), 
    void *dso_handle) 
{ 
    static_cast<void>(object); 
    static_cast<void>(destructor); 
    static_cast<void>(dso_handle); 
    return 0; 
}

void* __dso_handle = nullptr;

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