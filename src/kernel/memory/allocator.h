/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 13:24:45
 * LastEditTime : 2022-05-31 13:44:37
 * Description  : Heap allocator
 */

#ifndef _ALLOCATOR_H_
#define _ALLOCATOR_H_

#include "os_type.h"
#include "bitmap.h"

// void* malloc(int size, int amount);
// void free(void* addr);

#define HEAP_START_ADDR 0x100000

/**
 * Alloc specified amount of memory on heap and return the start address
 * 
 * @param size the size of memory unit
 * @param amount the amount of memory unit to be allocated
 */
void* malloc(int size, int amount);
/**
 * Free the allocated memory on heap
 * 
 * @param addr the start address of the allocated memory
 * @param amount the amount of memory to be freed
 */
void free(void* addr, int amount);

#endif