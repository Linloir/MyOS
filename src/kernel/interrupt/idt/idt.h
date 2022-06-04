/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 09:41:57
 * LastEditTime : 2022-06-04 15:23:09
 * Description  : Interrupt Descriptor & Interrupt Descriptor Table
 */

#ifndef _IDT_H_
#define _IDT_H_

#include "os_type.h"

#define IDT_MAX_SIZE 256
#define IDT_ADDR 0x8800

enum class DescriptorFlag {
    IS_TRAP     = 1 << 8,
    NOT_TASK    = 1 << 9,
    IS_32_BIT   = 1 << 11,
    PRESENT     = 1 << 15,
};

DescriptorFlag operator|(DescriptorFlag lhs, DescriptorFlag rhs);
DescriptorFlag operator&(DescriptorFlag lhs, DescriptorFlag rhs);
DescriptorFlag operator-(DescriptorFlag lhs, DescriptorFlag rhs);
DescriptorFlag& operator+=(DescriptorFlag& lhs, DescriptorFlag& rhs);
DescriptorFlag& operator-=(DescriptorFlag& lhs, DescriptorFlag& rhs);
DescriptorFlag operator!(DescriptorFlag flag);
bool contains(DescriptorFlag flagSet, DescriptorFlag flag);

class InterruptDescriptor {
    private:
        uint32 lo;
        uint32 hi;
    public:
        InterruptDescriptor(uint32 offset, uint16 selector, int ring, DescriptorFlag flags);
        DescriptorFlag flags();
        void setFlags(DescriptorFlag flags);
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
