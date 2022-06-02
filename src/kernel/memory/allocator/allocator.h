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

#define HEAP_START_ADDR 0xB000FF00

struct mem_control_block {
  int is_available;
  int size;
};

void malloc_init(void* lastValidAddr);
void* malloc(long numbytes);
void free(void* firstbyte);

#endif