/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:07:56
 * LastEditTime : 2022-06-07 17:54:32
 * Description  : 
 */

#include "framemanager.h"
#include "paging.h"
#include "linkedlist.h"
#include "algorithm.h"
#include "mmu.h"

FrameManager physicalFrames;

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
    }
    
    _allocatedFrames += skippedFrames;

    _lock.release();
}