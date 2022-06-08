/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 20:24:53
 * LastEditTime : 2022-06-08 13:17:42
 * Description  : 
 */

#include "os_type.h"

#ifndef _GDT_H_
#define _GDT_H_

#define GDT_MAX_SIZE 4

enum class GlobalDescriptorFlag {
    _EMPTY          = 0,
    ACCESSED        = 1 << 8,
    RW_AVAILABLE    = 1 << 9,
    GROWS_DOWN      = 1 << 10,
    EXECUTABLE      = 1 << 11,
    NOT_SYSTEM      = 1 << 12,
    PRESENT         = 1 << 15,
    USE_64_BIT      = 1 << 20,
    USE_32_BIT      = 1 << 21,
    USE_4KB_BLOCK   = 1 << 22,
};

GlobalDescriptorFlag operator|(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs);
GlobalDescriptorFlag operator&(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs);
GlobalDescriptorFlag operator-(GlobalDescriptorFlag lhs, GlobalDescriptorFlag rhs);
GlobalDescriptorFlag operator!(GlobalDescriptorFlag flag);
bool contains(GlobalDescriptorFlag flagSet, GlobalDescriptorFlag flag);

class GlobalDescriptor {
    private:
        uint64 val;
    public:
        GlobalDescriptor(): val(0) {}
        GlobalDescriptor(uint32 base, uint32 limit, int ring, GlobalDescriptorFlag flags);
        GlobalDescriptorFlag flags();
        void setFlags(GlobalDescriptorFlag flags);
        uint32 base();
        uint32 limit();
        int ring();
};

class GlobalDescriptorTable {
    private:
        GlobalDescriptor descriptors[GDT_MAX_SIZE];
    public:
        static GlobalDescriptorTable* fromPhysicalAddr(uint32 addr);
        static GlobalDescriptorTable* fromVirtualAddr(uint32 addr);
        GlobalDescriptorTable(){}
        void initialize();
        uint32 physicalAddr();
        uint32 virtualAddr();
        GlobalDescriptor& operator[](int index);
        GlobalDescriptor& getAt(int index);
        void setAt(int index, GlobalDescriptor descriptor);
        void removeAt(int index);
        void loadToGDTR();
};

extern GlobalDescriptorTable GDT;

extern "C" void asm_load_gdtr(uint32 addr);

#endif