/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 09:42:01
 * LastEditTime : 2022-06-16 22:17:09
 * Description  : Interrupt Descriptor structures
 */

#include "idt.h"
#include "handler.h"
#include "mmu.h"
#include "allocator.h"
#include "systemconstant.h"
#include "interrupt.h"
#include "syscallmanager.h"

InterruptDescriptorFlag operator|(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs) {
    return static_cast<InterruptDescriptorFlag>(static_cast<uint32>(lhs) | static_cast<uint32>(rhs));
}

InterruptDescriptorFlag operator&(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs) {
    return static_cast<InterruptDescriptorFlag>(static_cast<uint32>(lhs) & static_cast<uint32>(rhs));
}

InterruptDescriptorFlag operator-(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs) {
    return static_cast<InterruptDescriptorFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
}

InterruptDescriptorFlag& operator+=(InterruptDescriptorFlag& lhs, InterruptDescriptorFlag& rhs) {
    lhs = static_cast<InterruptDescriptorFlag>(static_cast<uint32>(lhs) + static_cast<uint32>(rhs));
    return lhs;
}

InterruptDescriptorFlag& operator-=(InterruptDescriptorFlag& lhs, InterruptDescriptorFlag& rhs) {
    lhs = static_cast<InterruptDescriptorFlag>(static_cast<uint32>(lhs) - static_cast<uint32>(rhs));
    return lhs;
}

InterruptDescriptorFlag operator!(InterruptDescriptorFlag flag) {
    return static_cast<InterruptDescriptorFlag>(~static_cast<uint32>(flag));
}

bool contains(InterruptDescriptorFlag flagSet, InterruptDescriptorFlag flag) {
    return (static_cast<uint32>(flagSet) & static_cast<uint32>(flag)) != 0;
}

InterruptDescriptor::InterruptDescriptor(uint32 offset, uint16 selector, int ring, InterruptDescriptorFlag flags) {
    uint32 preservedBits = 1 << 10;
    uint32 offsetHigh = offset & 0xFFFF0000;
    uint32 offsetLow = offset & 0x0000FFFF;
    uint32 sel = (uint32)selector << 16;
    uint32 r = ((uint32)ring & 0x3) << 13;  //restrict ring range
    uint64 hi = offsetHigh | static_cast<uint32>(flags) | r | preservedBits;
    uint64 lo = sel | offsetLow;
    val = (hi << 32) | lo;
}

InterruptDescriptorFlag InterruptDescriptor::flags() {
    uint32 hi = val >> 32;
    return (InterruptDescriptorFlag)hi & (InterruptDescriptorFlag::IS_TRAP | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::PRESENT);
}

void InterruptDescriptor::setFlags(InterruptDescriptorFlag flags) {
    InterruptDescriptorFlag all = InterruptDescriptorFlag::IS_TRAP | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::PRESENT;
    val &= (uint64)static_cast<uint32>(!all) << 32;
    val |= (uint64)static_cast<uint32>(flags) << 32;
}

uint32 InterruptDescriptor::offset() {
    uint32 offsetHigh = (val >> 32) & 0xFFFF0000;
    uint32 offsetLow = val & 0x0000FFFF;
    return offsetHigh | offsetLow;
}

int InterruptDescriptor::ring() {
    return ((val >> 32) >> 13) & 0x3;
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
        (uint32)emptyHandler, 
        CODE_SELECTOR, 
        0x0, 
        InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::PRESENT
    );
    for(int i = 0; i < IDT_MAX_SIZE; i++) {
        setAt(i, defaultDescriptor);
    }

    //Set overriden handlers
    InterruptDescriptor doubleFaultDescriptor = InterruptDescriptor(
        (uint32)doubleFaultInterruptHandler,
        CODE_SELECTOR,
        0x0,
        InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::PRESENT
    );
    setAt(0xD, doubleFaultDescriptor);

    InterruptDescriptor timeInterruptDescriptor = InterruptDescriptor(
        (uint32)timeInterruptHandler,
        CODE_SELECTOR,
        0x0,
        InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::PRESENT
    );
    setAt(0x20, timeInterruptDescriptor);

    // InterruptDescriptor pageFaultInterruptHandler = InterruptDescriptor(
    //     (uint32)asm_page_fault_interrupt_handler,
    //     CODE_SELECTOR,
    //     0x0,
    //     InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::PRESENT
    // );
    // setAt(0xE, pageFaultInterruptHandler);

    InterruptDescriptor syscallInterruptDescriptor = InterruptDescriptor(
        (uint32)syscallHandler,
        CODE_SELECTOR,
        0x3,
        InterruptDescriptorFlag::IS_32_BIT | InterruptDescriptorFlag::NOT_TASK | InterruptDescriptorFlag::PRESENT
    );
    setAt(0x80, syscallInterruptDescriptor);
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
    descriptors[index].setFlags(descriptors[index].flags() - InterruptDescriptorFlag::PRESENT);
}

void InterruptDescriptorTable::loadToIDTR() {
    byte idtr[6] = {0};
    uint16* tableLimit = (uint16*)idtr;
    uint32* base = (uint32*)(idtr + 2);
    *tableLimit = IDT_MAX_SIZE * 8 - 1;
    *base = virtualAddr();
    asm_load_idtr((uint32)idtr);
}