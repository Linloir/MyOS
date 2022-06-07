/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:56
 * LastEditTime : 2022-06-07 23:31:04
 * Description  : 
 */

#include "framemanager.h"
#include "paging.h"
#include "linkedlist.h"
#include "algorithm.h"
#include "mmu.h"

SemLock FrameManager::_lock = SemLock(1);
uint32 FrameManager::_tickTillRefresh = FrameManager::_defaultRefreshInterval;
uint32 FrameManager::_totalFrames = 0;
uint32 FrameManager::_freeFrames = 0;
Vec<Frame> FrameManager::_availableFrames;
Vec<Frame> FrameManager::_allocatedFrames;

void FrameManager::reclaimFrames(uint32 count) {
    _lock.acquire();
    if(count <= 0)  return;

    qsort(_allocatedFrames, cmp);
    Vec<Frame> skippedFrames;
    int reclaimedCount = 0;
    while(reclaimedCount < count && _allocatedFrames.size() > 0) {
        Frame f = _allocatedFrames.back();
        _allocatedFrames.popBack();
        if(contains(f.flags(), FrameFlag::LOCKED)) {
            skippedFrames.pushBack(f);
            continue;
        }
        SwapManager::swapOut(f);
        f.reclaim();
        _availableFrames.pushBack(f);
        reclaimedCount++;
        _freeFrames++;
    }
    
    _allocatedFrames += skippedFrames;

    _lock.release();
}

void FrameManager::initialize() {

}

Frame FrameManager::allocateFrame() {
    _lock.acquire();
    if(_freeFrames == 0) {
        _lock.release();
        return;
    }
    Frame alloc = _availableFrames.back();
    _availableFrames.popBack();
    _allocatedFrames.pushBack(alloc);
    _freeFrames--;
    _lock.release();
    return alloc;
}

Vec<Frame> FrameManager::allocateFrames(uint32 count) {
    _lock.acquire();
    if(count < _freeFrames) {
        _lock.release();
        reclaimFrames(count - _freeFrames);
        _lock.acquire();
        if(count < _freeFrames) {
            _lock.release();
            return;
        }
    }
    Vec<Frame> alloc;
    for(int i = 0; i < count; i++) {
        Frame newFrame = _availableFrames.back();
        _availableFrames.popBack();
        _allocatedFrames.pushBack(newFrame);
        alloc.pushBack(newFrame);
    }
    _freeFrames -= count;
    _lock.release();
    return alloc;
}

void FrameManager::freeFrame(uint32 physicalAddr) {
    _lock.acquire();
    for(int i = 0; i < _allocatedFrames.size(); i++) {
        if(_allocatedFrames[i].physicalAddr() != physicalAddr)  continue;
        
        _allocatedFrames[i].reclaim();
        _availableFrames.pushBack(_allocatedFrames[i]);
        _allocatedFrames.erase(i);
        break;
    }
    _freeFrames++;
    _lock.release();
}

void FrameManager::ageFrames() {
    _lock.acquire();
    for(int i = 0; i < _allocatedFrames.size(); i++) {
        _allocatedFrames[i].updateAccess();
    }
    _lock.release();
}