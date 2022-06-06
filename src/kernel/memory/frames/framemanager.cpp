/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:56
 * LastEditTime : 2022-06-06 22:23:14
 * Description  : 
 */

#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include "os_type.h"
#include "vector.h"
#include "frame.h"

class FrameManager {
    private:
        Vec<Frame*> _availableFrames;
        Vec<Frame*> _shadowFrames;
        Vec<Frame*> _allocatedFrames;
        Vec<Vec<uint32>> _swappedFrames;
        void reclaimFrames(uint32 count);
        void swapOut(Frame* frame);
        void swapIn(uint32 attemptingAddr);
    public:
        bool tryShadow(uint32 attemptingAddr, uint32 pid);
        bool trySwap(uint32 );
        Vec<Frame*> allocateFrames(uint32 count);
        void freeFrames(Frame* frame);
        void ageFrames();
};

#endif