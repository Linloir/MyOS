/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 21:27:18
 * LastEditTime : 2022-06-01 10:03:01
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
    void (*kernel)() = (void(*)())KERNEL_START_ADDR;
    kernel();
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

    //Init second level page table (set all entries to be 0(not present))
    for(int i = 0; i < 1024; i++) {
        scndLevelTable[i] = 0;
    }
    
    //Set the according entry of the virtual address 0MiB ~ 1MiB
    scndLevelTable[0] = FRST_LEVEL_TABLE_ADDR_BOOT | flag;
    //Set the according entry of the virtual address 3GiB ~ 3GiB + 1MiB
    scndLevelTable[virtAddr >> 22] = FRST_LEVEL_TABLE_ADDR_KERNEL | flag;

    enablePaging(SCND_LEVEL_TABLE_ADDR);
}