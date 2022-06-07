/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 17:34:34
 * LastEditTime : 2022-06-07 17:52:25
 * Description  : 
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "os_type.h"
#include "page.h"

enum class FrameFlag {
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