/*** 
 * Author       : Linloir
 * Date         : 2022-06-05 17:34:34
 * LastEditTime : 2022-06-06 22:03:50
 * Description  : 
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "os_type.h"
#include "page.h"

enum class FrameFlag {
    LOCKED      = 1,
    DIRTY       = 1 << 1,
};

class Frame {
    private:
        uint32 addr;
        PageTableEntry* pgEntry;
        FrameFlag flags;
        uint8 accessBits;
    public:
        void reclaim();
        void updateAccessHistory();
        uint8 accessHistory();
};

#endif