/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:53
 * LastEditTime : 2022-06-07 23:54:18
 * Description  : 
 */

#include "os_type.h"
#include "vector.h"
#include "frame.h"
#include "lock.h"

#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

class FrameManager {
    private:
        static const uint32 _defaultRefreshInterval = 500;

        static SemLock _lock;
        static uint32 _tickTillRefresh;

        static uint32 _totalFrames;
        static uint32 _freeFrames;

        static Vec<Frame> _availableFrames;
        static Vec<Frame> _allocatedFrames;

        static void reclaimFrames(uint32 count);
    public:
        static void initialize(int totalFrames, int mappedFrames);
        static Frame allocateFrame();
        static Vec<Frame> allocateFrames(uint32 count);
        static void freeFrame(uint32 physicalAddr);
        static void ageFrames();
};

#endif