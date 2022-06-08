/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:56
 * LastEditTime : 2022-06-08 11:08:50
 * Description  : 
 */

#include "framemanager.h"
#include "os_constant.h"
#include "paging.h"
#include "linkedlist.h"
#include "algorithm.h"
#include "mmu.h"

SemLock FrameManager::_lock;
uint32 FrameManager::_tickTillRefresh = FrameManager::_defaultRefreshInterval;
uint32 FrameManager::_totalFrames = 0;
uint32 FrameManager::_freeFrames = 0;
Vec<Frame> FrameManager::_availableFrames;
Vec<Frame> FrameManager::_allocatedFrames;

void FrameManager::reclaimFrames(uint32 count) {
    _lock.acquire();
    if(count <= 0)  return;

    qsort(_allocatedFrames, 0, _allocatedFrames.size() - 1, cmp);
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

void FrameManager::initialize(int totalFrames, int mappedFrames) {
    _totalFrames = totalFrames;
    _freeFrames = 0;
    uint32 addr = 0x0;
    uint32 mappedStart = toVirtualAddress(0x200000);
    uint32 mappedEnd = toVirtualAddress(0x200000 + mappedFrames * PAGE_SIZE);
    for(int i = 0; i < totalFrames; i++) {
        if(addr >= mappedStart && addr <= mappedEnd) {
            _allocatedFrames.pushBack(Frame(
                addr,
                FrameFlag::LOCKED,
                nullptr
            ));
        }
        else {
            _availableFrames.pushBack(Frame(
                addr,
                FrameFlag::EMPTY,
                nullptr
            ));
            _freeFrames++;
        }
    }

    _lock = SemLock(1);

    printf("_lock at 0x%x with permit %d\n", &_lock, _lock.permits());
}

Frame FrameManager::allocateFrame() {
    _lock.acquire();
    if(_freeFrames == 0) {
        _lock.release();
        reclaimFrames(1);
        _lock.acquire();
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
            return Vec<Frame>();
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