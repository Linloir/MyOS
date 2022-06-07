/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:06:15
 * LastEditTime : 2022-06-07 17:37:32
 * Description  : 
 */

#include "swap.h"

SwapRecord::SwapRecord(uint32 sector, PageFlag oldFlags) {
    sector <<= 12;
    sector &= 0xFFFFF000;
    oldFlags = oldFlags - PageFlag::PRESENT;
    val |= sector;
    val |= static_cast<uint32>(oldFlags);
}

uint32 SwapRecord::sector() {
    return val >> 12;
}

PageFlag SwapRecord::pageFlags() {
    return static_cast<PageFlag>(val & 0xFFF);
}

PageTableEntry* SwapRecord::toPageTableEntry(uint32 newAddress) {
    newAddress &= 0xFFFFF000;
    val &= 0x00000FFF;
    val |= newAddress;
    return (PageTableEntry*)this;
}

Vec<uint32> SwapManager::_freeSectors;

void SwapManager::initialize(uint32 totalSectors) {
    uint32 managable = totalSectors >> 3;
    for(int i = 0; i < managable; i++) {
        _freeSectors.pushBack(i << 3);
    }
}

void SwapManager::swapOut(Frame frame) {
    
}