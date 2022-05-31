/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:11
 * LastEditTime : 2022-05-31 11:50:14
 * Description  : Frame manager, including allocate physical frames and deallocate physical frames
 */

#ifndef _FRAME_H_
#define _FRAME_H_

#include "bitmap.h"

class FrameManager {
    private:
        BitMap framePool;
    public:
        FrameManager(BitMap initBitMap);
        /**
         * Allocate one frame from memory
         */
        uint32 allocateFrame();
        
};

#endif
