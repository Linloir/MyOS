/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 20:24:53
 * LastEditTime : 2022-06-13 15:49:09
 * Description  : 
 */

#include "os_type.h"

#ifndef _GDT_H_
#define _GDT_H_

#define GDT_MAX_SIZE 10

enum class GlobalDescriptorFlag {
    _EMPTY          = 0,
    //ACCESS BYTE--------------
    //NOT SYSTEM
    ACCESSED        = 1 << 8,
    RW_AVAILABLE    = 1 << 9,
    GROWS_DOWN      = 1 << 10,
    EXECUTABLE      = 1 << 11,
    //SYSTEM
    IS_TSS          = 1 << 8,
    IS_BUSY         = 1 << 9,
    IS_32_BIT       = 1 << 11,
    //END OF ACCESS BYTE-------
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
        static void initialize();
        GlobalDescriptorTable(){}
        uint32 physicalAddr();
        uint32 virtualAddr();
        GlobalDescriptor& operator[](int index);
        GlobalDescriptor& getAt(int index);
        void setAt(int index, GlobalDescriptor descriptor);
        void removeAt(int index);
        int append(GlobalDescriptor descriptor);
        void loadToGDTR();
};

extern GlobalDescriptorTable GDT;

extern "C" void asm_load_gdtr(uint32 addr);

#endif