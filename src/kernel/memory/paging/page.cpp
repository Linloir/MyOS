/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 16:40:35
 * LastEditTime : 2022-06-17 18:14:17
 * Description  : 
 */

#include "systemconstant.h"
#include "page.h"
#include "mmu.h"

PageFlag operator|(PageFlag lhs, PageFlag rhs) {
    return static_cast<PageFlag>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
}

PageFlag operator&(PageFlag lhs, PageFlag rhs) {
    return static_cast<PageFlag>(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
}

PageFlag operator-(PageFlag lhs, PageFlag rhs) {
    return static_cast<PageFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
}

PageFlag& operator+=(PageFlag& lhs, PageFlag& rhs) {
    lhs = static_cast<PageFlag>(static_cast<uint32>(lhs) + static_cast<uint32>(rhs));
    return lhs;
}

PageFlag& operator-=(PageFlag& lhs, PageFlag& rhs) {
    lhs = static_cast<PageFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
    return lhs;
}

PageFlag operator!(PageFlag flag) {
    return static_cast<PageFlag>(~static_cast<uint32>(flag));
}

bool contains(PageFlag flagSet, PageFlag flag) {
    return (static_cast<uint32>(flagSet) & static_cast<uint32>(flag)) != 0;
}

Page::Page(uint32 addr): _addr(addr) {}

uint32 Page::virtualAddr() {
    return _addr;
}

uint32 Page::physicalAddr() {
    return toPhysicalAddress(_addr);
}

Page Page::higher() {
    return Page(_addr + PAGE_SIZE);
}

Page Page::lower() {
    return Page(_addr - PAGE_SIZE);
}

PageTableEntry::PageTableEntry(uint32 address, PageFlag flags) {
    address &= 0xFFFFF000;
    val = address | static_cast<uint32>(flags);
}

bool PageTableEntry::operator==(const PageTableEntry& other) {
    return val == other.val;
}

uint32 PageTableEntry::value() {
    return val;
}

uint32 PageTableEntry::address() {
    return val & 0xFFFFF000;
}

PageFlag PageTableEntry::flags() {
    return (PageFlag)(val & 0xFFF);
}

void PageTableEntry::setValue(uint32 address, PageFlag flags) {
    address &= 0xFFFFF000;
    val = address | static_cast<uint32>(flags);
}

void PageTableEntry::setAddress(uint32 address) {
    uint32 temp = val;
    temp &= 0x00000FFF;
    address &= 0xFFFFF000;
    temp |= address;
    val = temp;
}

void PageTableEntry::setFlags(PageFlag flags) {
    uint32 temp = val;
    temp &= 0xFFFFF000;
    temp |= static_cast<uint32>(flags);
    val = temp;
}

void PageTableEntry::clearFlags(PageFlag flags) {
    PageFlag curFlag = this->flags();
    curFlag = curFlag - flags;
    this->setFlags(curFlag);
}

bool PageTableEntry::isPresent() {
    return contains(flags(), PageFlag::PRESENT);
}

void PageTableEntry::erase() {
    val = 0;
}

SwapRecord* PageTableEntry::toSwapRecord(uint32 startSector) {
    startSector <<= 12;
    val &= 0x00000FFF;
    val |= startSector;
    return (SwapRecord*)this;
}

PageTable* PageTable::fromPhysicalAddr(uint32 addr) {
    return (PageTable*)toVirtualAddress(addr);
}

PageTable* PageTable::fromVirtualAddr(uint32 addr) {
    return (PageTable*)addr;
}

PageTableEntry& PageTable::operator[](int index) {
    return entries[index];
}

PageTableEntry& PageTable::entryAt(int index) {
    return entries[index];
}

PageTableEntry& PageTable::entryOf(Page page) {
    PageTableEntry frstLevelTableEntry = entries[page.virtualAddr() >> 22];
    if(!frstLevelTableEntry.isPresent()) {
        return frstLevelTableEntry;
    }
    PageTable* frstLevelTable = PageTable::fromPhysicalAddr(frstLevelTableEntry.address());
    return frstLevelTable->entryAt((page.virtualAddr() << 10) >> 22);
}

uint32 PageTable::physicalAddr() {
    return toPhysicalAddress((uint32)this);
}

uint32 PageTable::virtualAddr() {
    return (uint32)this;
}

void PageTable::insertAt(int index, PageTableEntry entry) {
    entries[index] = entry;
}

void PageTable::clone(PageTable* table) {
    for(int i = 0; i < 1024; i++) {
        entries[i] = table->entries[i];
    }
}

int PageTable::emptySlot() {
    for(int i = 0; i < 1024; i++) {
        if(!entries[i].isPresent()) {
            return i;
        }
    }
    return -1;
}

void PageTable::removeAt(int index) {
    entries[index].erase();
}

void PageTable::removeAt(int start, int size) {
    int end = start + size;
    for(int i = start; i < end; i++) {
        entries[i].erase();
    }
}

void PageTable::clear() {
    removeAt(0, 1024);
}