/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:06:15
 * LastEditTime : 2022-06-13 19:34:36
 * Description  : 
 */

#include "swap.h"
#include "page.h"
#include "frame.h"
#include "mmu.h"

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

SemLock SwapManager::_lock = SemLock(1);
Vec<uint32> SwapManager::_freeSectors;

void SwapManager::initialize(uint32 totalSectors) {
    uint32 managable = totalSectors >> 3;
    for(int i = 0; i < managable; i++) {
        _freeSectors.pushBack(i << 3);
    }
}

void SwapManager::swapOut(Frame* frame) {
    _lock.acquire();
    uint32 startSector = _freeSectors.back();
    _freeSectors.popBack();
    _lock.release();
    
    uint32 virtAddr = toVirtualAddress(frame->physicalAddr());
    uint32 sector = startSector;
    for(int i = 0; i < 8; i++) {
        writeSector(sector, virtAddr);
        sector++;
        virtAddr += 512;
    }
    
    frame->pageEntry()->clearFlags(PageFlag::PRESENT);
    frame->pageEntry()->toSwapRecord(startSector);
}

void SwapManager::swapIn(SwapRecord* record, Frame* newFrame) {
    uint32 virtAddr = toVirtualAddress(newFrame->physicalAddr());
    uint32 sector = record->sector();
    for(int i = 0; i < 8; i++) {
        readSector(sector, virtAddr);
        sector++;
        virtAddr += 512;
    }

    _lock.acquire();
    _freeSectors.pushBack(record->sector());
    _lock.release();

    PageTableEntry* entry = record->toPageTableEntry(virtAddr);
    entry->setFlags(entry->flags() | PageFlag::PRESENT);
    newFrame->setPageEntry(entry);
}