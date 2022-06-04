/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 09:42:01
 * LastEditTime : 2022-06-04 15:21:08
 * Description  : Interrupt Descriptor structures
 */

#include "idt.h"
#include "handler.h"
#include "mmu.h"
#include "allocator.h"
#include "os_constant.h"
#include "interrupt.h"

DescriptorFlag operator|(DescriptorFlag lhs, DescriptorFlag rhs) {
    return static_cast<DescriptorFlag>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
}

DescriptorFlag operator&(DescriptorFlag lhs, DescriptorFlag rhs) {
    return static_cast<DescriptorFlag>(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
}

DescriptorFlag operator-(DescriptorFlag lhs, DescriptorFlag rhs) {
    return static_cast<DescriptorFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
}

DescriptorFlag& operator+=(DescriptorFlag& lhs, DescriptorFlag& rhs) {
    lhs = static_cast<DescriptorFlag>(static_cast<uint32>(lhs) + static_cast<uint32>(rhs));
    return lhs;
}

DescriptorFlag& operator-=(DescriptorFlag& lhs, DescriptorFlag& rhs) {
    lhs = static_cast<DescriptorFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
    return lhs;
}

DescriptorFlag operator!(DescriptorFlag flag) {
    return static_cast<DescriptorFlag>(~static_cast<uint32>(flag));
}

bool contains(DescriptorFlag flagSet, DescriptorFlag flag) {
    return (static_cast<uint32>(flagSet) & static_cast<uint32>(flag)) != 0;
}

InterruptDescriptor::InterruptDescriptor(uint32 offset, uint16 selector, int ring, DescriptorFlag flags) {
    uint32 preservedBits = 1 << 10;
    uint32 offsetHigh = offset & 0xFFFF0000;
    uint32 offsetLow = offset & 0x0000FFFF;
    uint32 sel = (uint32)selector << 16;
    uint32 r = ((uint32)ring & 0x3) << 13;  //restrict ring range
    hi = offsetHigh | static_cast<uint32>(flags) | r;
    lo = sel | offsetLow;
}

DescriptorFlag InterruptDescriptor::flags() {
    return (DescriptorFlag)hi & (DescriptorFlag::IS_TRAP | DescriptorFlag::NOT_TASK | DescriptorFlag::IS_32_BIT | DescriptorFlag::PRESENT);
}

void InterruptDescriptor::setFlags(DescriptorFlag flags) {
    DescriptorFlag all = DescriptorFlag::IS_TRAP | DescriptorFlag::NOT_TASK | DescriptorFlag::IS_32_BIT | DescriptorFlag::PRESENT;
    hi &= static_cast<uint32>(!all);
    hi |= static_cast<uint32>(flags);
}

uint32 InterruptDescriptor::offset() {
    uint32 offsetHigh = hi & 0xFFFF0000;
    uint32 offsetLow = lo & 0x0000FFFF;
    return offsetHigh | offsetLow;
}

int InterruptDescriptor::ring() {
    return (hi >> 13) & 0x3;
}

InterruptDescriptorTable* InterruptDescriptorTable::fromPhysicalAddr(uint32 addr) {
    return (InterruptDescriptorTable*)toVirtualAddress(addr);
}

InterruptDescriptorTable* InterruptDescriptorTable::fromVirtualAddr(uint32 addr) {
    return (InterruptDescriptorTable*)addr;
}

void InterruptDescriptorTable::initialize() {
    //Set default descriptor
    InterruptDescriptor defaultDescriptor = InterruptDescriptor(
        (uint32)asm_empty_interrupt_handler, 
        CODE_SELECTOR, 
        0x0, 
        DescriptorFlag::IS_32_BIT | DescriptorFlag::NOT_TASK | DescriptorFlag::PRESENT
    );
    for(int i = 0; i < IDT_MAX_SIZE; i++) {
        setAt(i, defaultDescriptor);
    }

    //Set overriden handlers
    InterruptDescriptor timeInterruptHandler = InterruptDescriptor(
        (uint32)asm_time_interrupt_handler,
        CODE_SELECTOR,
        0x0,
        DescriptorFlag::IS_32_BIT | DescriptorFlag::NOT_TASK | DescriptorFlag::PRESENT
    );
    setAt(IRQ0_8259A_MASTER, timeInterruptHandler);

    InterruptDescriptor pageFaultInterruptHandler = InterruptDescriptor(
        (uint32)asm_page_fault_interrupt_handler,
        CODE_SELECTOR,
        0x0,
        DescriptorFlag::IS_32_BIT | DescriptorFlag::NOT_TASK | DescriptorFlag::PRESENT
    );
    setAt(0xE, pageFaultInterruptHandler);
}

uint32 InterruptDescriptorTable::physicalAddr() {
    return toPhysicalAddress((uint32)this);
}

uint32 InterruptDescriptorTable::virtualAddr() {
    return (uint32)this;
}

InterruptDescriptor& InterruptDescriptorTable::operator[](int index) {
    return descriptors[index];
}

InterruptDescriptor& InterruptDescriptorTable::getAt(int index) {
    return descriptors[index];
}

void InterruptDescriptorTable::setAt(int index, InterruptDescriptor descriptor) {
    descriptors[index] = descriptor;
}

void InterruptDescriptorTable::removeAt(int index) {
    descriptors[index].setFlags(descriptors[index].flags() - DescriptorFlag::PRESENT);
}

void InterruptDescriptorTable::loadToIDTR() {
    byte* addr = (byte*)malloc(sizeof(byte) * 6);
    uint16* tableLimit = (uint16*)addr;
    uint32* base = (uint32*)(addr + 2);
    *tableLimit = IDT_MAX_SIZE;
    *base = virtualAddr();
    asm_load_idtr((uint32)addr);
}