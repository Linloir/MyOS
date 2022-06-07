/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 17:34:38
 * LastEditTime : 2022-06-07 17:53:11
 * Description  : 
 */

#include "frame.h"

FrameFlag operator|(FrameFlag lhs, FrameFlag rhs) {
    return static_cast<FrameFlag>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
}

FrameFlag operator&(FrameFlag lhs, FrameFlag rhs) {
    return static_cast<FrameFlag>(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
}

FrameFlag operator-(FrameFlag lhs, FrameFlag rhs) {
    return static_cast<FrameFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
}

FrameFlag operator!(FrameFlag flag) {
    return static_cast<FrameFlag>(~static_cast<uint32>(flag));
}

bool contains(FrameFlag flagSet, FrameFlag flag) {
    return (static_cast<uint32>(flagSet) & static_cast<uint32>(flag)) != 0;
}

Frame::Frame(uint32 physicalAddr, FrameFlag flags, PageTableEntry* entry) {
    physicalAddr &= 0xFFFFF000;
    uint32 f = static_cast<uint32>(flags);
    _val = physicalAddr | f;
    _pageEntry = entry;
    _access = 0;
}

uint32 Frame::physicalAddr() {
    return _val & 0xFFFFF000;
}

FrameFlag Frame::flags() {
    return static_cast<FrameFlag>(_val & 0xFFF);
}

void Frame::setFlags(FrameFlag flags) {
    _val &= 0xFFFFF000;
    uint32 f = static_cast<uint32>(flags);
    f &= 0xFFF;
    _val |= f;
}

PageTableEntry* Frame::pageEntry() {
    return _pageEntry;
}

void Frame::setPageEntry(PageTableEntry* entry) {
    _pageEntry = entry;
}
uint8 Frame::access() {
    return _access;
}

void Frame::updateAccess() {
    _access >>= 1;
    uint8 newBit = contains(_pageEntry->flags(), PageFlag::ACCESSED) ? 1 : 0;
    newBit <<= 7;
    _access |= newBit;
    _pageEntry->clearFlags(PageFlag::ACCESSED);
}

void Frame::reclaim() {
    _pageEntry = nullptr;
    _val &= 0xFFFFF000;
    _access = 0;
}

bool cmp(Frame& a, Frame& b) {
    return a.access() >= b.access();
}