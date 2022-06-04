/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 14:03:08
 * LastEditTime : 2022-06-03 21:27:26
 * Description  : 
 */

#include "mmu.h"

uint32 toVirtualAddress(uint32 addr){
    return addr + OFFSET;
}

uint32 toPhysicalAddress(uint32 addr) {
    return addr - OFFSET;
}