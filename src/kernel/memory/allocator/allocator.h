/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 13:24:45
 * LastEditTime : 2022-06-06 16:19:39
 * Description  : Heap allocator
 */

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "os_type.h"

struct mem_control_block {
  int is_available;
  int size;
};

void malloc_init(void* last_valid_address);
void* malloc(long numbytes);
void free(void* firstbyte);

#endif