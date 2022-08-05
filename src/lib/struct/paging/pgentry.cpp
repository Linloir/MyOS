/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 19:14:46
 * LastEditTime : 2022-08-04 19:52:14
 * Description  : Page Table Entry
 */

#include "pgentry.h"

PageFlag::PageFlag() {
    _attr = 0;
}
PageFlag::PageFlag(uint16 flags) {
    _attr = flags;
}
PageFlag& PageFlag::operator=(uint16 flags) {
    _attr = flags;
    return *this;
}
PageFlag& PageFlag::operator=(const PageFlag& other) {
    _attr = other._attr;
    return *this;
}
//Judge whether two flags are equal
bool PageFlag::operator==(uint16 flags) {
    return _attr == flags;
}
//Combine two flags
PageFlag PageFlag::operator+(uint16 flags) {
    uint16 newAttr = _attr | flags;
    return PageFlag(newAttr);
}
PageFlag& PageFlag::operator+=(uint16 flags) {
    _attr |= flags;
    return *this;
}
//Remove flags
PageFlag PageFlag::operator-(uint16 flags) {
    uint16 newAttr = _attr & ~(flags);
    return PageFlag(newAttr);
}
PageFlag& PageFlag::operator-=(uint16 flags) {
    _attr &= ~(flags);
    return *this;
}
//Bitwise or
PageFlag PageFlag::operator|(uint16 flags) {
    uint16 newAttr = _attr | flags;
    return PageFlag(newAttr);
}
PageFlag& PageFlag::operator|=(uint16 flags) {
    _attr |= flags;
    return *this;
}
//Bitwise and
PageFlag PageFlag::operator&(uint16 flags) {
    uint8 newAttr = _attr & flags;
    return PageFlag(newAttr);
}
PageFlag& PageFlag::operator&=(uint16 flags) {
    _attr &= flags;
    return *this;
}
//Reverse flags
PageFlag PageFlag::operator~() {
    return PageFlag(_attr);
}
//Toggle flag bits
PageFlag& PageFlag::operator^=(uint16 flags) {
    _attr ^= flags;
    return *this;
}
//Judge whether flags are contained
bool PageFlag::contains(uint16 flags) {
    return (_attr & flags) == flags;
}
uint16 PageFlag::val() {
    return _attr;
}

PageTableEntry* PageTableEntry::from(uint32 addr) {
    return (PageTableEntry*)addr;
}

PageTableEntry::PageTableEntry(uint32 addr, PageFlag flags) {
    _val = addr & 0xFFFFF000;
    _val |= (flags.val() & 0xFFF);
}

bool PageTableEntry::isPresent() const {
    return flags().contains(PageFlag::PRESENT);
}

uint32 PageTableEntry::address() const {
    return _val & 0xFFFFF000;
}

void PageTableEntry::setAddress(uint32 addr) {
    _val &= 0x00000FFF;
    _val |= (addr & 0xFFFFF000);
}

PageFlag PageTableEntry::flags() const {
    return _val & 0xFFF;
}

void PageTableEntry::setFlags(PageFlag flags) {
    _val &= 0xFFFFF000;
    _val |= (flags.val() & 0xFFF);
}