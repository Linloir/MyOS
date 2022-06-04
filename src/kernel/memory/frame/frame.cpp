/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 20:17:19
 * LastEditTime : 2022-06-04 14:46:02
 * Description  : 
 */

#include "frame.h"
#include "mmu.h"
#include "allocator.h"

FrameManager FrameManager::physicalFrames = FrameManager(0);

void FrameManager::initialize(int totalFrames, int mappedFrames) {
    //Init frame pool
    physicalFrames.framePool = BitMap(totalFrames);
    //Init frame pool
    uint32 startFrame = 256;
    uint32 endFrame = startFrame + mappedFrames;
    for(int i = startFrame; i < endFrame; i++) {
        physicalFrames.framePool.set(i, true);
    }
}

FrameManager::FrameManager(int size): framePool(BitMap(size)) {}

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
    int frameIndex = framePool.alloc();
    if(frameIndex == -1) {
        //Swap frame
        return 0;
    }
    return frameIndex << 12;
}

//@TODO To be finished
uint32* FrameManager::allocateFrames(int count) {
    uint32* framesAddr = (uint32*)malloc(sizeof(uint32) * count);
    return framesAddr;
}

//@TODO To be finished
uint32 FrameManager::allocateSequencialFrames(int count) {
    return 0;
}

void FrameManager::freeFrame(uint32 physicalAddr) {
    int frameIndex = physicalAddr >> 12;
    framePool.free(frameIndex);
}