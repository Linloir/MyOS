/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 09:41:57
 * LastEditTime : 2022-06-04 20:58:22
 * Description  : Interrupt Descriptor & Interrupt Descriptor Table
 */

#ifndef _IDT_H_
#define _IDT_H_

#include "os_type.h"

#define IDT_MAX_SIZE 256
#define IDT_ADDR 0x8800

enum class InterruptDescriptorFlag {
    IS_TRAP     = 1 << 8,
    NOT_TASK    = 1 << 9,
    IS_32_BIT   = 1 << 11,
    PRESENT     = 1 << 15,
};

InterruptDescriptorFlag operator|(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs);
InterruptDescriptorFlag operator&(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs);
InterruptDescriptorFlag operator-(InterruptDescriptorFlag lhs, InterruptDescriptorFlag rhs);
InterruptDescriptorFlag& operator+=(InterruptDescriptorFlag& lhs, InterruptDescriptorFlag& rhs);
InterruptDescriptorFlag& operator-=(InterruptDescriptorFlag& lhs, InterruptDescriptorFlag& rhs);
InterruptDescriptorFlag operator!(InterruptDescriptorFlag flag);
bool contains(InterruptDescriptorFlag flagSet, InterruptDescriptorFlag flag);

class InterruptDescriptor {
    private:
        uint64 val;
    public:
        InterruptDescriptor(uint32 offset, uint16 selector, int ring, InterruptDescriptorFlag flags);
        InterruptDescriptorFlag flags();
        void setFlags(InterruptDescriptorFlag flags);
        uint32 offset();
        int ring();
};

class InterruptDescriptorTable {
    private:
        InterruptDescriptor descriptors[IDT_MAX_SIZE];
    public:
        static InterruptDescriptorTable* fromPhysicalAddr(uint32 addr);
        static InterruptDescriptorTable* fromVirtualAddr(uint32 addr);
        void initialize();
        uint32 physicalAddr();
        uint32 virtualAddr();
        InterruptDescriptor& operator[](int index);
        InterruptDescriptor& getAt(int index);
        void setAt(int index, InterruptDescriptor descriptor);
        void removeAt(int index);
        void loadToIDTR();
};

extern "C" void asm_load_idtr(uint32 addr);

#endif
