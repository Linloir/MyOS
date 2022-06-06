/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 17:34:34
 * LastEditTime : 2022-06-06 15:52:00
 * Description  : 
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "os_type.h"
#include "page.h"

enum class FrameFlag {
    FREE        = 1,
    LOCKED      = 1 << 1,
    REFERENCED  = 1 << 2,
    IS_FILE     = 1 << 3,
};

class Frame {
    private:
        uint32 addr;
        PageTableEntry* pgEntry;
        FrameFlag flags;
    public:
        void reclaim();
};

#endif