/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 16:40:32
 * LastEditTime : 2022-06-07 17:25:53
 * Description  : 
 */

#ifndef _PAGE_H_
#define _PAGE_H_

#include "os_type.h"
#include "swap.h"

enum class PageFlag{
    PRESENT         = 1,
    WRITABLE        = 1 << 1,
    USER_ACCESSIBLE = 1 << 2,
    WRITE_THROUGH   = 1 << 3,
    NO_CACHE        = 1 << 4,
    ACCESSED        = 1 << 5,
    DIRTY           = 1 << 6,
    HUGE_PAGE       = 1 << 7,
    GLOBAL          = 1 << 8,
    BIT_9           = 1 << 9,
    BIT_10          = 1 << 10,
    BIT_11          = 1 << 11,
    _ALL            = 0xFFF,
};

PageFlag operator|(PageFlag lhs, PageFlag rhs);
PageFlag operator&(PageFlag lhs, PageFlag rhs);
PageFlag operator-(PageFlag lhs, PageFlag rhs);
PageFlag& operator+=(PageFlag& lhs, PageFlag& rhs);
PageFlag& operator-=(PageFlag& lhs, PageFlag& rhs);
PageFlag operator!(PageFlag flag);
bool contains(PageFlag flagSet, PageFlag flag);

class PageTableEntry {
    private:
        uint32 val;
    public:
        PageTableEntry(uint32 address, PageFlag flags);
        uint32 value();
        uint32 address();
        PageFlag flags();
        void setValue(uint32 address, PageFlag flags);
        void setAddress(uint32 address);
        void setFlags(PageFlag flags);
        void clearFlags(PageFlag flags);
        bool isPresent();
        void erase();
        SwapRecord* toSwapRecord(uint32 startSector);
};

class PageTable {
    private:
        PageTableEntry entries[1024];
    public:
        static PageTable* fromVirtualAddr(uint32 virtualAddress);
        static PageTable* fromPhysicalAddr(uint32 virtualAddress);
        PageTableEntry& operator[](int index);
        PageTableEntry& entryAt(int index);
        uint32 physicalAddr();
        uint32 virtualAddr();
        void insertAt(int index, PageTableEntry entry);
        void removeAt(int index);
        void removeAt(int start, int size);
        void clear();
};

#endif
