#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "os_type.h"
#include "interrupt_utils.h"
#include "display_utils.h"
#include "os_constant.h"

class InterruptManager{
private:
    uint32* descriptorTable;
    void (*timeInterruptHandlerFunc)();
public:
    static const uint16 IDT_MAX_SIZE = 256;
    static const uint8 IRQ0_8259A_MASTER = 0x20;
    static const uint8 IRQ0_8259A_SLAVE = 0x28;
    InterruptManager(uint32* tableAddr);
    void setDescriptor(InterruptDescriptor newDescriptor, uint32 id);
    void set8259A();
    void enableTimeInterrupt();
    void disableTimeInterrupt();
    void setTimeInterruptHandler(void (*handler)());
    bool getInterruptStatus();
    void setInterruptStatus(bool status);
};

class InterruptDescriptor{
public:
    uint16 handlerSegmentSelector;
    uint32 handlerOffset;
    uint8 pBit;             // 1 for exist
    uint8 dplBits;          // Priviledge level; 0 - 3 descending
    uint8 dBit;             // 1 for 32-bit code
    InterruptDescriptor(uint16 selector, uint32 offset, uint8 p, uint8 dpl, uint8 d);
};

extern "C"
void emptyHandler();

extern "C"
void timeInterruptHandler();

#endif