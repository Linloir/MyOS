/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:21
 * LastEditTime : 2022-05-17 14:49:59
 * Description  : 
 */
#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "os_type.h"
#include "interrupt_utils.h"
#include "display_utils.h"
#include "os_constant.h"

class InterruptDescriptor{
public:
    uint16 handlerSegmentSelector;
    uint32 handlerOffset;
    uint8 pBit;             // 1 for exist
    uint8 dplBits;          // Priviledge level; 0 - 3 descending
    uint8 dBit;             // 1 for 32-bit code
    InterruptDescriptor(uint16 selector, void (*offset)(), uint8 p, uint8 dpl, uint8 d);
};

class InterruptManager {
private:
    static uint32* descriptorTable;
    static void (*timeInterruptHandlerFunc)();
public:
    static const uint16 IDT_MAX_SIZE = 256;
    static const uint8 IRQ0_8259A_MASTER = 0x20;
    static const uint8 IRQ0_8259A_SLAVE = 0x28;
    static void initialize(uint32* tableAddr);
    static void setDescriptor(InterruptDescriptor newDescriptor, uint32 id);
    static void set8259A();
    static void enableTimeInterrupt();
    static void disableTimeInterrupt();
    static void setTimeInterruptHandler(void (*handler)());
    static bool getInterruptStatus();
    static void setInterruptStatus(bool status);
};

extern "C"
void emptyHandler();

extern "C"
void timeInterruptHandler();

#endif