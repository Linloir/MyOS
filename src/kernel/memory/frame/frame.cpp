/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:19
 * LastEditTime : 2022-05-31 16:56:17
 * Description  : 
 */

#include "frame.h"

BitMap FrameManager::framePool = BitMap(0);

void FrameManager::initialize(int memorySize) {
    //Get total managable frame number
    int totalFrames = memorySize >> 12;
    //Init frame pool
    framePool = BitMap(totalFrames);
    for(int i = 0; i < 256; i++) {
        framePool.set(i, true);
    }
}

inline int FrameManager::availableFrames() {
    return framePool.availableResources();
}

inline int FrameManager::occupiedFrames() {
    return framePool.unavailableResources();
}

inline int FrameManager::totalFrames() {
    return framePool.totalResources();
}

uint32 FrameManager::allocateFrame() {
    int pageIndex = framePool.alloc();
    if(pageIndex == -1) {
        //Swap frame
        return 0;
    }
    return pageIndex << 12;
}