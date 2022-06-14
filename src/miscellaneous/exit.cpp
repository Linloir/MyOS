/*** 
 * Author       : Linloir
 * Date         : 2022-06-08 10:25:10
 * LastEditTime : 2022-06-14 19:51:20
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