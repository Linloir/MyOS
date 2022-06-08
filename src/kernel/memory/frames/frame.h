/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 17:34:34
 * LastEditTime : 2022-06-08 08:58:07
 * Description  : 
 */

#include "os_type.h"
#include "page.h"

#ifndef _FRAME_H_
#define _FRAME_H_

enum class FrameFlag {
    EMPTY      = 0,
    LOCKED      = 1,
};

FrameFlag operator|(FrameFlag lhs, FrameFlag rhs);
FrameFlag operator&(FrameFlag lhs, FrameFlag rhs);
FrameFlag operator-(FrameFlag lhs, FrameFlag rhs);
FrameFlag operator!(FrameFlag flag);
bool contains(FrameFlag flagSet, FrameFlag flag);

class Frame {
    private:
        uint32 _val;
        PageTableEntry* _pageEntry;
        uint8 _access = 0;
    public:
        Frame(uint32 physicalAddr, FrameFlag flags, PageTableEntry* entry);
        
        uint32 physicalAddr();

        FrameFlag flags();
        void setFlags(FrameFlag flags);
        
        PageTableEntry* pageEntry();
        void setPageEntry(PageTableEntry* entry);

        uint8 access();
        void updateAccess();

        void reclaim();
};

bool cmp(Frame& a, Frame& b);

#endif