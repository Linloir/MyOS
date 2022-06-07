/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:53
 * LastEditTime : 2022-06-07 17:54:04
 * Description  : 
 */

#ifndef _FRAME_MANAGER_H_
#define _FRAME_MANAGER_H_

#include "os_type.h"
#include "vector.h"
#include "frame.h"
#include "lock.h"

class FrameManager {
    private:
        const uint32 _minAvailableFrames = 50;

        SemLock _lock;
        uint32 _tickTillRefresh;

        uint32 _totalFrames;
        uint32 _freeFrames;

        Vec<Frame> _availableFrames;
        Vec<Frame> _allocatedFrames;

        void reclaimFrames(uint32 count);
    public:
        FrameManager();
        FrameManager(uint32 totalFrames);
        Frame allocateFrame();
        Vec<Frame> allocateFrames(uint32 count);
        void freeFrame(Frame frame);
        void ageFrames();
};

extern FrameManager physicalFrames;

extern "C" void readSector(int sector, uint32 virtualAddr);
extern "C" void writeSector(int sector, uint32 virtualAddr);

#endif