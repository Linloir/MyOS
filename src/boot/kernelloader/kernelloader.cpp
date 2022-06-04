/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 21:27:18
 * LastEditTime : 2022-06-04 17:17:08
 * Description  : Kernel loader
 */

#include "kernelloader.h"
#include "os_constant.h"

/**
 * Kernel loader:
 *  - loads the kernel from hard disk
 *  - set initial pages
 *  - enable paging
 *  - jump to kernel
 */
extern "C" void kernelLoader() {
    loadKernel();
    initializePaging();
    jumpKernel(0xFFFFFFFE, KERNEL_START_ADDR);
}

void loadKernel() {
    int sectorIndex = KERNEL_START_SECTOR;
    uint32 loadAddr = 0x100000;     //1MiB
    for(int i = 0; i < KERNEL_SECTOR_COUNT; i++) {
        readSector(sectorIndex, loadAddr);
        sectorIndex++;
        loadAddr += 512;
    }
}

void initializePaging() {
    uint32* scndLevelTable = (uint32*)SCND_LEVEL_TABLE_ADDR;
    uint32* frstLevelTableBoot = (uint32*)FRST_LEVEL_TABLE_ADDR_BOOT;
    uint32* frstLevelTableKernel = (uint32*)FRST_LEVEL_TABLE_ADDR_KERNEL;
    uint32 physAddr = 0x0;          //0KiB
    uint32 virtAddr = 0xB0000000;   //3GiB
    uint32 flag = 0x3;              //P, RWX, PRIVILEDGE

    //Init second level page table (set all entries to be 0(not present))
    for(int i = 0; i < 1024; i++) {
        scndLevelTable[i] = 0;
    }
    
    //Set the according entry of the virtual address 0MiB ~ 1MiB
    scndLevelTable[0] = FRST_LEVEL_TABLE_ADDR_BOOT | flag;
    //Set the according entry of the virtual address 3GiB ~ 3GiB + 1MiB
    scndLevelTable[virtAddr >> 22] = FRST_LEVEL_TABLE_ADDR_KERNEL | flag;

    //Map Memory 0MiB ~ 1MiB -> 0MiB ~ 1MiB
    for(int i = 0; i < 256; i++) {
        frstLevelTableBoot[i] = physAddr | flag;
        physAddr += PAGE_SIZE;
    }

    //Map Memory 3GiB ~ 3GiB + 1MiB -> 1MiB ~ 2MiB
    for(int i = 0; i < 256; i++) {
        frstLevelTableKernel[i] = physAddr | flag;
        physAddr += PAGE_SIZE;
    }

    int tableCount = 3;

    //Map Memory 3GiB + 1MiB ~ 3GiB + 1MiB + SIZEOF(MEMORY) -> 0MiB ~ SIZEOF(MEMORY)
    uint32 memorySize = *(uint32*)0x7c00;
    uint32 frameCount = memorySize >> 12;
    uint32* table = (uint32*)FRST_LEVEL_TABLE_ADDR_KERNEL;
    int entryIndex = 256;
    physAddr = 0x0;
    virtAddr = 0xB0100000;
    for(int i = 0; i < frameCount; i++) {
        table[entryIndex] = physAddr | flag;
        physAddr += PAGE_SIZE;
        virtAddr += PAGE_SIZE;
        entryIndex++;
        if(entryIndex == 1024) {
            entryIndex = 0;
            table = (uint32*)((uint32)table + 0x1000);  //table = table + 4KiB
            scndLevelTable[virtAddr >> 22] = (uint32)table | flag;
            tableCount++;
        }
    }

    //Map pages for heap initialization
    uint32 heapAddr = virtAddr;
    int initHeapFrameCount = 9;
    
    for(int i = 0; i < initHeapFrameCount; i++) {
        if(entryIndex == 1024) {
            entryIndex = 0;
            table = (uint32*)((uint32)table + 0x1000);  //table = table + 4KiB
            scndLevelTable[virtAddr >> 22] = (uint32)table | flag;
            tableCount++;
        }
        table[entryIndex] = physAddr | flag;
        physAddr += PAGE_SIZE;
        virtAddr += PAGE_SIZE;
        entryIndex++;
    }

    //Map stack frame
    uint32 stackFrameAddr = 0xFFE00000;
    int stackFrameCount = 1 << 9;
    table = (uint32*)((uint32)table + 0x1000);
    tableCount++;
    entryIndex = (stackFrameAddr << 10) >> 22;
    for(int i = 0; i < stackFrameCount; i++) {
        table[entryIndex] = physAddr | flag;
        physAddr += PAGE_SIZE;
        entryIndex++;
    }
    scndLevelTable[stackFrameAddr >> 22] = (uint32)table | flag;

    //Map first heap page and store page information
    uint32* tempHeap = (uint32*)0x0;
    tempHeap[0] = memorySize;
    tempHeap[1] = frameCount;   //Total frame count
    tempHeap[2] = (frameCount + (1 << 10) - 1) >> 10;   //Frame used to map whole memory
    tempHeap[3] = SCND_LEVEL_TABLE_ADDR;
    tempHeap[4] = heapAddr;
    tempHeap[5] = initHeapFrameCount;
    tempHeap[6] = stackFrameCount;
    tempHeap[7] = tableCount;

    enablePaging(SCND_LEVEL_TABLE_ADDR);
}