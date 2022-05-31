/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 13:24:45
 * LastEditTime : 2022-05-31 16:35:18
 * Description  : Heap allocator
 */

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "os_type.h"
#include "bitmap.h"

// void* malloc(int size, int amount);
// void free(void* addr);

#define HEAP_START_ADDR 0x0FF000

struct mem_control_block {
  int is_available;
  int size;
};

void malloc_init(void* lastValidAddr);
void* malloc(long numbytes);
void free(void* firstbyte);

#endif