/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:43:15
 * LastEditTime : 2022-06-07 23:44:20
 * Description  : Paging
 */

#include "paging.h"
#include "framemanager.h"
#include "mmu.h"

void PageManager::initialize() {
    //Free unused page
    PageTable* scndLevelTable = PageTable::fromPhysicalAddr(getCR3());
    PageTable* bootTable = PageTable::fromPhysicalAddr(scndLevelTable->entryAt(0).address());
    scndLevelTable->removeAt(0);
    bootTable->removeAt(0, 256);
}

PageTableEntry* PageManager::mapPage(uint32 virtualAddr, uint32 physicalAddr, PageFlag flags) {
    //fetch l2 table
    PageTable* scndLevelTable = PageTable::fromPhysicalAddr(getCR3());
    uint32 frstLevelTableIndex = virtualAddr >> 22;
    //create l1 table if not exist
    if(!scndLevelTable->entryAt(frstLevelTableIndex).isPresent()) {
        uint32 newTableAddr = FrameManager::allocateFrame().physicalAddr();
        PageTableEntry newEntry = PageTableEntry(newTableAddr, PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE);
        scndLevelTable->insertAt(frstLevelTableIndex, newEntry);
    }
    //add entry to l1 table
    PageTable* frstLevelTable = PageTable::fromPhysicalAddr(scndLevelTable->entryAt(frstLevelTableIndex).address());
    uint32 entryIndex = (virtualAddr << 10) >> 22;
    PageTableEntry newEntry = PageTableEntry(physicalAddr, flags);
    frstLevelTable->insertAt(entryIndex, newEntry);
    return &(frstLevelTable->entryAt(entryIndex));
}

uint32 PageManager::unmapPage(uint32 virtualAddr) {
    //fetch l2 table
    PageTable* scndLevelTable = PageTable::fromPhysicalAddr(getCR3());
    uint32 frstLevelTableIndex = virtualAddr >> 22;
    //check existence of l1 table
    if(!scndLevelTable->entryAt(frstLevelTableIndex).isPresent()) {
        return toPhysicalAddress(virtualAddr);
    }
    //remove entry from l1 table
    PageTable* frstLevelTable = PageTable::fromPhysicalAddr(scndLevelTable->entryAt(frstLevelTableIndex).address());
    uint32 entryIndex = (virtualAddr << 10) >> 22;
    frstLevelTable->removeAt(entryIndex);
    return toPhysicalAddress(virtualAddr);
}