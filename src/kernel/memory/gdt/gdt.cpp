/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 20:24:58
 * LastEditTime : 2022-06-04 22:45:58
 * Description  : 
 */

#include "gdt.h"
#include "mmu.h"
#include "allocator.h"

GlobalDescriptorTable GDT;

GlobalDescriptorFlag operator|(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs) {
    return static_cast<GlobalDescriptorFlag>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
}

GlobalDescriptorFlag operator&(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs) {
    return static_cast<GlobalDescriptorFlag>(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
}

GlobalDescriptorFlag operator-(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs) {
    return static_cast<GlobalDescriptorFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
}

GlobalDescriptorFlag operator!(GlobalDescriptorFlag flag) {
    return static_cast<GlobalDescriptorFlag>(~static_cast<uint32>(flag));
}

bool contains(GlobalDescriptorFlag flagSet, GlobalDescriptorFlag flag) {
    return (static_cast<uint32>(flagSet) & static_cast<uint32>(flag)) != 0;
}

GlobalDescriptor::GlobalDescriptor(uint32 base, uint32 limit, int ring, GlobalDescriptorFlag flags) {
    uint64 base_hi = (base & 0xFF000000) << 32;
    uint64 base_mid = (base & 0x00FF0000) << (32 - 16);
    uint64 base_lo = (base & 0x0000FFFF) << 16;
    uint64 limit_hi = (limit & 0x000F0000) << 32;
    uint64 limit_lo = (limit & 0x0000FFFF);
    uint64 r = (uint64)(ring & 0x3) << 45;
    if(contains(flags, GlobalDescriptorFlag::USE_64_BIT | GlobalDescriptorFlag::USE_32_BIT)) {
        flags = flags - GlobalDescriptorFlag::USE_32_BIT;
    }
    uint64 f = (uint64)static_cast<uint32>(flags) << 32;
    val = base_hi | base_mid | base_lo | limit_hi | limit_lo | r | f;
}

GlobalDescriptorFlag GlobalDescriptor::flags() {
    uint32 flags = val >> 32;
    return (GlobalDescriptorFlag)flags;
}

void GlobalDescriptor::setFlags(GlobalDescriptorFlag flags) {
    GlobalDescriptorFlag all = GlobalDescriptorFlag::ACCESSED | GlobalDescriptorFlag::RW_AVAILABLE | GlobalDescriptorFlag::GROWS_DOWN |
                                GlobalDescriptorFlag::EXECUTABLE | GlobalDescriptorFlag::SYSTEM | GlobalDescriptorFlag::PRESENT | 
                                GlobalDescriptorFlag::USE_64_BIT | GlobalDescriptorFlag::USE_32_BIT | GlobalDescriptorFlag::USE_4KB_BLOCK;
    uint64 f = (uint64)static_cast<uint32>(flags) << 32;
    val &= ~((uint64)static_cast<uint32>(all) << 32);
    val |= f;
}

uint32 GlobalDescriptor::base() {
    uint32 base_hi = (val >> 32) & 0xFF000000;
    uint32 base_mid = (val >> 16) & 0x00FF0000;
    uint32 base_lo = (val >> 16) & 0x0000FFFF;
    return base_hi | base_mid | base_lo;
}

uint32 GlobalDescriptor::limit() {
    uint32 limit_hi = (val >> 32) & 0x000F0000;
    uint32 limit_lo = val & 0x0000FFFF;
    return limit_hi | limit_lo;
}

int GlobalDescriptor::ring() {
    return (int)(val >> 45) & 0x3;
}

GlobalDescriptorTable* GlobalDescriptorTable::fromPhysicalAddr(uint32 addr) {
    return (GlobalDescriptorTable*)toVirtualAddress(addr);
}

GlobalDescriptorTable* GlobalDescriptorTable::fromVirtualAddr(uint32 addr) {
    return (GlobalDescriptorTable*)addr;
}

void GlobalDescriptorTable::initialize() {
    //Empty Segment
    GlobalDescriptor emptySegmentDescriptor = GlobalDescriptor(
        0x0,
        0x0,
        0,
        GlobalDescriptorFlag::_EMPTY
    );
    descriptors[0] = emptySegmentDescriptor;
    //Code Segment
    GlobalDescriptor codeSegmentDescriptor = GlobalDescriptor(
        0x0,
        0xFFFFF,
        0,
        GlobalDescriptorFlag::EXECUTABLE | GlobalDescriptorFlag::PRESENT | 
        GlobalDescriptorFlag::USE_32_BIT | GlobalDescriptorFlag::USE_4KB_BLOCK  
    );
    descriptors[1] = codeSegmentDescriptor;

    //Data Segment
    GlobalDescriptor dataSegmentDescriptor = GlobalDescriptor(
        0x0,
        0xFFFFF,
        0,
        GlobalDescriptorFlag::RW_AVAILABLE | GlobalDescriptorFlag::PRESENT | 
        GlobalDescriptorFlag::USE_32_BIT | GlobalDescriptorFlag::USE_4KB_BLOCK
    );
    descriptors[2] = dataSegmentDescriptor;

    //Stack Segment
    GlobalDescriptor stackSegmentDescriptor = GlobalDescriptor(
        0x0,
        0x0000,
        0,
        GlobalDescriptorFlag::GROWS_DOWN | GlobalDescriptorFlag::RW_AVAILABLE | 
        GlobalDescriptorFlag::PRESENT | GlobalDescriptorFlag::USE_32_BIT | 
        GlobalDescriptorFlag::USE_4KB_BLOCK
    );
    descriptors[3] = stackSegmentDescriptor;
}

uint32 GlobalDescriptorTable::physicalAddr() {
    return toPhysicalAddress((uint32)this);
}

uint32 GlobalDescriptorTable::virtualAddr() {
    return (uint32)this;
}

GlobalDescriptor& GlobalDescriptorTable::operator[](int index) {
    return descriptors[index];
}

GlobalDescriptor& GlobalDescriptorTable::getAt(int index) {
    return descriptors[index];
}

void GlobalDescriptorTable::setAt(int index, GlobalDescriptor descriptor) {
    descriptors[index] = descriptor;
}

void GlobalDescriptorTable::removeAt(int index) {
    descriptors[index].setFlags(descriptors[index].flags() - GlobalDescriptorFlag::PRESENT);
}

void GlobalDescriptorTable::loadToGDTR() {
    byte gdtr[6] = {0};
    uint16* limit = (uint16*)gdtr;
    uint32* base = (uint32*)(gdtr + 2);
    *limit = GDT_MAX_SIZE * 8 - 1;
    *base = virtualAddr();
    asm_load_gdtr((uint32)gdtr);
}