/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 13:24:34
 * LastEditTime : 2022-06-03 15:24:01
 * Description  : 
 */

#include "allocator.h"
#include "std_utils.h"

int has_initialized = 0;
void *managed_memory_start;
void *last_valid_address;

void malloc_init(void* addr) {
    last_valid_address = addr;
    managed_memory_start = last_valid_address;
    has_initialized = 1;
}

void free(void *firstbyte) {
    struct mem_control_block *mcb;
    mcb = (struct mem_control_block *)((uint32)firstbyte - sizeof(struct mem_control_block));
    mcb->is_available = 1;
    return;
}

void *malloc(long numbytes) {
    void *current_location;
    struct mem_control_block *current_location_mcb;
    void *memory_location;

    if (!has_initialized) {
        return 0;
    }

    numbytes = numbytes + sizeof(struct mem_control_block);
    memory_location = 0;
    current_location = managed_memory_start;

    while (current_location != last_valid_address) {
        current_location_mcb = (struct mem_control_block *)current_location;

        if (current_location_mcb->is_available) {
            if (current_location_mcb->size >= numbytes) {
                current_location_mcb->is_available = 0;
                memory_location = current_location;
                break;
            }
        }

        current_location = (void *)((uint32)current_location + current_location_mcb->size);
    }
    if (!memory_location) {
        memory_location = last_valid_address;

        last_valid_address = (void *)((uint32)last_valid_address + numbytes);

        current_location_mcb = (struct mem_control_block *)memory_location;
        current_location_mcb->is_available = 0;
        current_location_mcb->size = numbytes;
    }

    memory_location = (void *)((uint32)memory_location + sizeof(struct mem_control_block));

    return memory_location;
}