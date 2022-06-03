/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 14:03:08
 * LastEditTime : 2022-06-03 14:05:59
 * Description  : 
 */

#include "mmu.h"

uint32 physicToVirtual(uint32 physicAddr) {
    return physicAddr + OFFSET;
}

uint32 VirtualToPhysic(uint32 virtualAddr) {
    return virtualAddr - OFFSET;
}