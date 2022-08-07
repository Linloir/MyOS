/*** 
 * Author       : Linloir
 * Date         : 2022-08-07 15:32:39
 * LastEditTime : 2022-08-07 20:57:56
 * Description  : Boot Information
 */

#include "types.h"

#ifndef _BOOT_INFO_H_
#define _BOOT_INFO_H_

class BootInfo {
    public:
        uint32 memorySize;
        uint32 CR3;
        uint32 allocPhysMemStart;
        uint32 allocPhysMemEnd;
        uint32 kernelPhysAddr;
        uint32 kernelVirtAddr;
        uint32 kernelSize;
        uint32 kernelEntry;
        uint32 memoryMappingOffset;
        uint32 heapPhysAddr;
        uint32 heapVirtAddr;
        uint32 heapInitSize;
        uint32 stackPhysAddr;
        uint32 stackVirtAddr;
        uint32 stackInitSize;
};

#endif
