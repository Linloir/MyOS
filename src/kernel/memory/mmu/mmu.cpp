/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 14:03:08
 * LastEditTime : 2022-06-13 22:17:31
 * Description  : 
 */

#include "mmu.h"
#include "os_constant.h"

uint32 toVirtualAddress(uint32 addr){
    return addr + OFFSET;
}

uint32 toPhysicalAddress(uint32 addr) {
    return addr - OFFSET;
}

uint32 pageAlignUpper(uint32 addr) {
    return ((addr + PAGE_SIZE - 1) / PAGE_SIZE) * PAGE_SIZE;
}

uint32 pageAlignLower(uint32 addr) {
    return (addr / PAGE_SIZE) * PAGE_SIZE;
}