/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:19
 * LastEditTime : 2022-06-01 09:28:01
 * Description  : 
 */

#include "frame.h"

BitMap FrameManager::framePool = BitMap(0);

void FrameManager::initialize(int memorySize) {
    //Get total managable frame number
    int totalFrames = memorySize >> 12;
    //Init frame pool
    framePool = BitMap(totalFrames);
    //Init 1MiB ~ 2MiB memory
    for(int i = 256; i < 512; i++) {
        framePool.set(i, true);
    }
    //Init 2MiB ~ 2MiB + 4KiB memory (L2 table)
    framePool.set(512, true);
    //Init 2MiB + 8KiB memory ~ 2MiB + 12KiB memory (L1 table kernel)
    framePool.set(514, true);
}

int FrameManager::availableFrames() {
    return framePool.availableResources();
}

int FrameManager::occupiedFrames() {
    return framePool.unavailableResources();
}

int FrameManager::totalFrames() {
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