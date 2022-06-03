/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:19
 * LastEditTime : 2022-06-03 15:08:33
 * Description  : 
 */

#include "frame.h"
#include "mmu.h"

BitMap FrameManager::framePool = BitMap(0);

void FrameManager::initialize() {
    //Get total managable frame number
    int* info = (int*)physicToVirtual(MEMORY_INFO_ADDR);
    int totalFrames = info[1];
    //Init frame pool
    framePool = BitMap(totalFrames);
    //Init frame pool
    uint32 startFrame = 256;
    uint32 kernelFrame = 256;
    uint32 pageInitFrame = 2;
    uint32 pageMappingFrame = info[2];
    uint32 heapFrame = 1;
    uint32 endFrame = startFrame + kernelFrame + pageInitFrame + pageMappingFrame + heapFrame;
    for(int i = startFrame; i < endFrame; i++) {
        framePool.set(i, true);
    }
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