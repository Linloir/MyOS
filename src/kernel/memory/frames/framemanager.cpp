/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:56
 * LastEditTime : 2022-06-17 15:12:25
 * Description  : 
 */

#include "framemanager.h"
#include "systemconstant.h"
#include "paging.h"
#include "linkedlist.h"
#include "algorithm.h"
#include "mmu.h"
#include "systemio.h"

SpinLock FrameManager::_lock;
uint32 FrameManager::_tickTillRefresh = FrameManager::_defaultRefreshInterval;
uint32 FrameManager::_totalFrames = 0;
uint32 FrameManager::_freeFrames = 0;
Vec<Frame*> FrameManager::_availableFrames;
Vec<Frame*> FrameManager::_allocatedFrames;
Frame* FrameManager::_allFrames;

void FrameManager::reclaimFrames(uint32 count) {
    _lock.lock();
    if(count <= 0)  return;

    qsort(_allocatedFrames, 0, _allocatedFrames.size() - 1, cmp);
    Vec<Frame*> skippedFrames;
    int reclaimedCount = 0;
    while(reclaimedCount < count && _allocatedFrames.size() > 0) {
        Frame* f = _allocatedFrames.front();
        _allocatedFrames.erase(0);
        if(contains(f->flags(), FrameFlag::LOCKED)) {
            skippedFrames.pushBack(f);
            continue;
        }
        SwapManager::swapOut(f);
        f->reclaim();
        _availableFrames.pushBack(f);
        reclaimedCount++;
        _freeFrames++;
    }
    
    _allocatedFrames += skippedFrames;

    _lock.release();
}

void FrameManager::initialize(int totalFrames, int mappedFrames) {
    _totalFrames = totalFrames;
    _freeFrames = 0;
    _availableFrames = Vec<Frame*>(_totalFrames);
    _allocatedFrames = Vec<Frame*>();
    uint32 addr = 0x0;
    uint32 mappedStart = 0x100000;
    uint32 mappedEnd = 0x200000 + mappedFrames * PAGE_SIZE;
    Frame* newFrameArr = (Frame*)malloc(sizeof(Frame) * totalFrames);
    if(newFrameArr == nullptr) {
        return;
    }
    _allFrames = newFrameArr;
    for(int i = 0; i < totalFrames; i++) {
        Frame* newFrame = &newFrameArr[i];
        if(addr >= mappedStart && addr <= mappedEnd) {
            *newFrame = Frame(
                addr,
                FrameFlag::LOCKED,
                nullptr
            );
            _allocatedFrames.pushBack(newFrame);
        }
        else {
            *newFrame = Frame(
                addr,
                FrameFlag::EMPTY,
                nullptr
            );
            _availableFrames.pushBack(newFrame);
            _freeFrames++;
        }
        addr += PAGE_SIZE;
    }

    _lock = SpinLock();
    
    printf("Frame Manager Initialized!, free frames: %d\n", _freeFrames);
}

Frame* FrameManager::allocateFrame(FrameFlag flags) {
    _lock.lock();
    if(_freeFrames == 0) {
        _lock.release();
        reclaimFrames(1);
        _lock.lock();
    }
    Frame* alloc = _availableFrames.front();
    alloc->setFlags(flags);
    _availableFrames.erase(0);
    _allocatedFrames.pushBack(alloc);
    _freeFrames--;
    _lock.release();
    return alloc;
}

Vec<Frame*> FrameManager::allocateFrames(uint32 count, FrameFlag flags) {
    _lock.lock();
    if(count > _freeFrames) {
        _lock.release();
        reclaimFrames(count - _freeFrames);
        _lock.lock();
        if(count > _freeFrames) {
            _lock.release();
            return Vec<Frame*>();
        }
    }
    Vec<Frame*> alloc;
    for(int i = 0; i < count; i++) {
        Frame* newFrame = _availableFrames.front();
        newFrame->setFlags(flags);
        _availableFrames.erase(0);
        _allocatedFrames.pushBack(newFrame);
        alloc.pushBack(newFrame);
    }
    _freeFrames -= count;
    _lock.release();
    return alloc;
}

void FrameManager::freeFrame(uint32 physicalAddr) {
    _lock.lock();
    Frame* frame = &_allFrames[physicalAddr >> 12];
    if(!frame->pageEntry()->isPresent()) {
        _lock.release();
        return;
    }
    frame->reclaim();
    _allocatedFrames.erase(frame);
    _availableFrames.pushBack(frame);
    _lock.release();
}

void FrameManager::ageFrames() {
    _lock.lock();
    for(int i = 0; i < _allocatedFrames.size(); i++) {
        _allocatedFrames[i]->updateAccess();
    }
    _lock.release();
}