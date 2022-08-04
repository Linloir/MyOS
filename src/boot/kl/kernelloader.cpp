/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 22:09:52
 * LastEditTime : 2022-08-04 12:23:53
 * Description  : 
 */

#include "kernelloader.h"

extern "C" void kernelLoader() {
    uint32 kernelSize = readKernel();
    // PageTable* pageTable = enablePaging();
    // loadKernel(pageTable, kernelSize);
    boot();
}

uint32 readKernel() {
    MBR mbr;
    HDDManager().readSector(0, 1, (byte*)&mbr);
    if(mbr.partition_1.isPresent() && mbr.partition_1.isActive()) {
        return readFromPartition(mbr.partition_1);
    }
    else if(mbr.partition_2.isPresent() && mbr.partition_2.isActive()) {
        return readFromPartition(mbr.partition_2);
    }
    else if(mbr.partition_3.isPresent() && mbr.partition_3.isActive()) {
        return readFromPartition(mbr.partition_3);
    }
    else if(mbr.partition_4.isPresent() && mbr.partition_4.isActive()) {
        return readFromPartition(mbr.partition_4);
    }
    else {
        //Print err message
        while(true) {}
    }
    return 0;
}

PageTable* enablePaging() {
    return (PageTable*)(0);
}

void loadKernel(PageTable* pgtable, uint32 kernelSize) {

}

void boot() {
    void (*kernel)() = (void (*)())KERNEL_LOAD_PHYS_ADDR;
    kernel();
}

uint32 readFromPartition(Partition p) {
    //Read root directory from bpb
    FATBPB bpb = FATBPB(p.firstSectorLBA());
    uint32 rootSector = bpb.isFAT32() ? 
                        bpb.absoluteSecOfCluster(bpb.rootCluster()) : 
                        bpb.rootDirFrstSecAbsolute();
    uint32 systemDirClus = locateSysFolder(rootSector);
    uint32 kernelFrstClus = locateKernelFile(bpb.absoluteSecOfCluster(systemDirClus));
    return readKernelFile(bpb, kernelFrstClus);
}

uint32 locateSysFolder(uint32 rootSector) {
    //Note: Since we will make sure that the entry of /system/ folder
    //in the first sector of the root directory, therefore there's no
    //need to consider the actual size of the directory table. Instead, 
    //only the first 512 bytes of the directory table will be buffered
    byte rootEntries[512];
    HDDManager().readSector(rootSector, 1, rootEntries);
    for(int i = 0; i < 16; i++) {
        DirEntry* entry = (DirEntry*)(&rootEntries[i * 32]);
        //Entry should not be empty
        if(entry->isEmpty()) continue;
        //Entry should be SFN entry
        if(entry->isLFNEntry()) continue;
        //Entry should indicate a directory
        if(!entry->attributes().contains(DirectoryFlag::DIRECTORY)) continue;
        //Entry should have name as "SYSTEM     "
        byte name[12];
        memset(name, 0, 12);
        entry->parseASCIIFileName(name);
        if(strcmp((char*)name, "SYSTEM     ") != 0) continue;
        //Read the first cluster out of the entry
        return entry->firstCluster();
    }
    return 0;
}

uint32 locateKernelFile(uint32 systemDirSec) {
    //Note: Similar to root directory, the kernel file will be made
    //sure to stay in the first sector of the system directory table
    //Therefore also only the first 512 bytes of the directory table
    //will be buffered
    byte systemEntries[512];
    HDDManager().readSector(systemDirSec, 1, systemEntries);
    for(int i = 0; i < 16; i++) {
        DirEntry* entry = (DirEntry*)(&systemEntries[i * 32]);
        //Entry should not be empty
        if(entry->isEmpty()) continue;
        //Entry should be SFN entry
        if(entry->isLFNEntry()) continue;
        //Entry should not indicate a directory
        if(entry->attributes().contains(DirectoryFlag::DIRECTORY)) continue;
        //Entry should have name as "KERNEL     "
        byte name[12];
        memset(name, 0, 12);
        entry->parseASCIIFileName(name);
        if(strcmp((char*)name, "KERNEL  BIN") != 0) continue;
        //Read the first cluster of the entry
        return entry->firstCluster();
    }
    return 0;
}

uint32 readKernelFile(FATBPB& bpb, uint32 kernelFrstClus) {
    //After extracting the first cluster of kernel file, the FAT table
    //is needed to read the entire kernel file from disk. Since the API
    //for FAT will be implemented in the kernel with buffer management,
    //which is highly dependent on malloc function, we won't have any
    //API to use for FAT reading.
    //
    //Without any APIs, the allocation and release of the buffer are
    //done on the stack, whose space is rather valuable, therefore only
    //one sector is buffered at a time for one entry of the FAT
    //
    //a 1024-byte buffer is used to buffer FAT and kernel size in byte
    //is stored in kernelSize variable
    byte fatBuffer[1024];
    //kernelLoadPhysAddr and loadOffset are used to store loaded kernel file
    byte* kernelLoadPhysAddr = (byte*)KERNEL_LOAD_PHYS_ADDR;
    uint32 loadOffset = 0;
    uint32 currentCluster = kernelFrstClus;
    uint32 currentFATSector = 0xFFFFFFFF;
    uint32 fatBaseSector = bpb.fatFrstSecAbsolute();
    uint32 bytsPerSector = bpb.bytesPerSector();
    uint32 fatEntrySize = bpb.fatEntrySize();
    uint32 eof = fatEntrySize == 12 ? 0xFFF : 
                fatEntrySize == 16 ? 0xFFFF :
                0xFFFFFFF;
    while(currentCluster != eof) {
        //Read current cluster into memory
        uint32 curSec = bpb.absoluteSecOfCluster(currentCluster);
        HDDManager().readSector(curSec, 1, kernelLoadPhysAddr + loadOffset);
        loadOffset += bytsPerSector;
        //Calculate next cluster
        uint32 fatEntryBitOffset = fatEntrySize * currentCluster;
        uint32 fatEntrySecOffset = fatEntryBitOffset / (bytsPerSector * 8);
        uint32 fatEntrySec = fatBaseSector + fatEntrySecOffset;
        if(currentFATSector != fatEntrySec) {
            //Reload buffer needed
            HDDManager().readSector(fatEntrySec, 2, fatBuffer);
            currentFATSector = fatEntrySec;
        }
        //Calculate the offset inside the sector
        uint32 sectorBitOffset = fatEntryBitOffset - fatEntrySecOffset * bytsPerSector * 8;
        uint32 sectorByteOffset = sectorBitOffset / 8;
        uint32 nextCluster = 0;
        //Read next cluster from FAT
        if(fatEntrySize == 12) {
            nextCluster = *(uint16*)(&fatBuffer[sectorByteOffset]);
            if(currentCluster & 1) {
                //Cluster number is odd
                nextCluster >>= 4;
            }
            else {
                nextCluster &= 0x0FFF;
            }
        }
        else if(fatEntrySize == 16) {
            nextCluster = *(uint16*)(&fatBuffer[sectorByteOffset]);
        }
        else {
            nextCluster = *(uint32*)(&fatBuffer[sectorByteOffset]);
        }
        currentCluster = nextCluster;
    }
    return loadOffset;
}