/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:43:15
 * LastEditTime : 2022-06-14 12:26:17
 * Description  : Paging
 */

#include "paging.h"
#include "framemanager.h"
#include "mmu.h"
#include "std_utils.h"

void PageManager::initialize() {
    //Free unused page
    PageTable* scndLevelTable = PageTable::fromPhysicalAddr(getCR3());
    PageTable* bootTable = PageTable::fromPhysicalAddr(scndLevelTable->entryAt(0).address());
    scndLevelTable->removeAt(0);
    bootTable->removeAt(0, 256);

    printf("Page Manager Initialized!\n");
}

PageTableEntry* PageManager::_mapPage(PageTable* pageTable, uint32 virtualAddr, uint32 physicalAddr, PageFlag flags) {
    //fetch l2 table
    PageTable* scndLevelTable = pageTable;
    uint32 frstLevelTableIndex = virtualAddr >> 22;
    //create l1 table if not exist
    if(!scndLevelTable->entryAt(frstLevelTableIndex).isPresent()) {
        uint32 newTableAddr = FrameManager::allocateFrame(FrameFlag::LOCKED)->physicalAddr();
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

PageTableEntry* PageManager::_mapPage(uint32 virtualAddr, uint32 physicalAddr, PageFlag flags) {
    PageTable* pageTable = PageTable::fromPhysicalAddr(getCR3());
    return _mapPage(pageTable, virtualAddr, physicalAddr, flags);
}

PageTableEntry* PageManager::mapPage(Page page, Frame* frame, PageFlag flags) {
    uint32 virtualAddr = page.virtualAddr();
    uint32 physicalAddr = frame->physicalAddr();
    PageTableEntry* entry = _mapPage(virtualAddr, physicalAddr, flags);
    frame->setPageEntry(entry);
    return entry;
}

PageTableEntry* PageManager::mapPage(PageTable* pageTable, Page page, Frame* frame, PageFlag flags) {
    uint32 virtualAddr = page.virtualAddr();
    uint32 physicalAddr = frame->physicalAddr();
    PageTableEntry* entry = _mapPage(pageTable, virtualAddr, physicalAddr, flags);
    frame->setPageEntry(entry);
    return entry;
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

uint32 PageManager::unmapPage(Page page) {
    return unmapPage(page.virtualAddr());
}

bool PageManager::isPageMapped(Page page) {
    uint32 virtualAddr = page.virtualAddr();
    PageTable* scndLevelTable = PageTable::fromPhysicalAddr(getCR3());
    uint32 frstLevelTableIndex = virtualAddr >> 22;
    if(!scndLevelTable->entryAt(frstLevelTableIndex).isPresent()) {
        return false;
    }
    PageTable* frstLevelTable = PageTable::fromPhysicalAddr(scndLevelTable->entryAt(frstLevelTableIndex).address());
    uint32 entryIndex = (virtualAddr << 10) >> 22;
    return frstLevelTable->entryAt(entryIndex).isPresent();
}