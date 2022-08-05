/*** 
 * Author       : Linloir
 * Date         : 2022-07-31 23:28:17
 * LastEditTime : 2022-08-04 19:51:15
 * Description  : 
 */

#include "types.h"

#ifndef _PGENTRY_H_
#define _PGENTRY_H_

class PageFlag {
    public:
        enum ATTRIBUTES {    
            _EMPTY          = 0,
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
    private:
        uint16 _attr;
    public:
        PageFlag();
        PageFlag(uint16 flags);
    public:
        PageFlag& operator=(uint16 flags);
        PageFlag& operator=(const PageFlag& other);
        //Judge whether two flags are equal
        bool operator==(uint16 flags);
        //Combine two flags
        PageFlag operator+(uint16 flags);
        PageFlag& operator+=(uint16 flags);
        //Remove flags
        PageFlag operator-(uint16 flags);
        PageFlag& operator-=(uint16 flags);
        //Bitwise or
        PageFlag operator|(uint16 flags);
        PageFlag& operator|=(uint16 flags);
        //Bitwise and
        PageFlag operator&(uint16 flags);
        PageFlag& operator&=(uint16 flags);
        //Reverse flags
        PageFlag operator~();
        //Toggle flag bits
        PageFlag& operator^=(uint16 flags);
        //Judge whether flags are contained
        bool contains(uint16 flags);
        uint16 val();
};

class PageTableEntry {
    private:
        uint32 _val = 0;
    public:
        static PageTableEntry* from(uint32 addr);
        PageTableEntry() {}
        PageTableEntry(uint32 addr, PageFlag flags);
        bool isPresent() const;
        uint32 address() const;
        void setAddress(uint32 addr);
        PageFlag flags() const;
        void setFlags(PageFlag flags);
};

#endif