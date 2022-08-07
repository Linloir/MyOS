/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 22:09:52
 * LastEditTime : 2022-08-07 23:26:31
 * Description  : 
 */

#include "kernelloader.h"

BootInfo bootInfo;

extern "C" void kernelLoader() {
    memset(&bootInfo, 0, sizeof(bootInfo));

    bootInfo.memorySize = *(uint32*)(0x7C00);
    alignDn(bootInfo.memorySize, PAGE_SIZE);

    readKernel();
    initPageTable();
    loadKernel();
    allocPages();
    enablePaging();
    boot();
}

void readKernel() {
    MBR mbr;
    HDDManager().readSector(0, 1, (byte*)&mbr);
    if(mbr.partition_1.isPresent() && mbr.partition_1.isActive()) {
        readFromPartition(mbr.partition_1);
    }
    else if(mbr.partition_2.isPresent() && mbr.partition_2.isActive()) {
        readFromPartition(mbr.partition_2);
    }
    else if(mbr.partition_3.isPresent() && mbr.partition_3.isActive()) {
        readFromPartition(mbr.partition_3);
    }
    else if(mbr.partition_4.isPresent() && mbr.partition_4.isActive()) {
        readFromPartition(mbr.partition_4);
    }
    else {
        //Print err message
        while(true) {}
    }
}

void initPageTable() {
    // Alloc main table
    uint32 mainTableAddress = allocPageTable();
    bootInfo.CR3 = mainTableAddress;
}

void loadKernel() {
    Elf32Header* kernel = Elf32Header::from(KERNEL_LOAD_PHYS_ADDR);
    bootInfo.kernelVirtAddr = 0xFFFFFFFF;
    bootInfo.kernelSize = 0;
    bootInfo.kernelEntry = kernel->entry();
    if(!kernel->isValid()) {
        //panic
        while(true) {}
    }
    uint32 segmentCnt = kernel->segmentCnt();
    for(int i = 0; i < segmentCnt; i++) {
        Elf32ProgramHeader* seg = kernel->segmentAt(i);
        loadSegment(PageTable::from(bootInfo.CR3), seg);
    }
}

void allocPages() {    
    // Map first 1MiB memory
    memMap(
        0x0, 
        0x0, 
        0x100000, 
        PageFlag::PRESENT | PageFlag::WRITABLE
    );

    //Map spaces for memory mapping
    bootInfo.memoryMappingOffset = bootInfo.kernelVirtAddr + bootInfo.kernelSize;
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    alignUp(bootInfo.memoryMappingOffset, PAGE_SIZE);
    memMap(
        0x0, 
        bootInfo.memoryMappingOffset,
        bootInfo.memorySize,
        PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
    );

    // Map pages for heap initialization
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    bootInfo.heapPhysAddr = bootInfo.allocPhysMemEnd;
    bootInfo.heapVirtAddr = bootInfo.memoryMappingOffset + bootInfo.memorySize;
    bootInfo.heapInitSize = 16 * PAGE_SIZE;
    bootInfo.allocPhysMemEnd += bootInfo.heapInitSize;
    alignUp(bootInfo.heapVirtAddr, PAGE_SIZE);
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    memMap(
        bootInfo.heapPhysAddr,
        bootInfo.heapVirtAddr,
        bootInfo.heapInitSize,
        PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
    );

    // Map spaces reserved for stack
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    bootInfo.stackPhysAddr = bootInfo.allocPhysMemEnd;
    bootInfo.stackVirtAddr = STACK_INIT_ADDR;
    bootInfo.stackInitSize = 0 - STACK_INIT_ADDR;
    alignUp(bootInfo.stackVirtAddr, PAGE_SIZE);
    alignDn(bootInfo.stackInitSize, PAGE_SIZE);
    bootInfo.allocPhysMemEnd += bootInfo.stackInitSize;
    memMap(
        bootInfo.stackPhysAddr,
        bootInfo.stackVirtAddr,
        bootInfo.stackInitSize,
        PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
    );

    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
}

void enablePaging() {
    uint32 mainTableAddress = bootInfo.CR3;
    // Enable paging
    asm volatile(
        "movl %[table], %%cr3\n\t"
        "movl %%cr0, %%eax\n\t"
        "orl $0x80000000, %%eax\n\t"
        "movl %%eax, %%cr0"
        : 
        : [table]"r"(mainTableAddress)
        : 
    );
}

void boot() {
    //move esp
    asm volatile(
        "movl $0, %esp"
    );
    //jump to kernel
    void (*kernel)(BootInfo&) = (void (*)(BootInfo&))bootInfo.kernelEntry;
    kernel(bootInfo);
}

void readFromPartition(Partition p) {
    //Read root directory from bpb
    FATBPB bpb = FATBPB(p.firstSectorLBA());
    uint32 rootSector = bpb.isFAT32() ? 
                        bpb.absoluteSecOfCluster(bpb.rootCluster()) : 
                        bpb.rootDirFrstSecAbsolute();
    uint32 systemDirClus = locateSysFolder(rootSector);
    uint32 kernelFrstClus = locateKernelFile(bpb.absoluteSecOfCluster(systemDirClus));
    readKernelFile(bpb, kernelFrstClus);
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
        if(strcmp((char*)name, "KERNEL  ELF") != 0) continue;
        //Read the first cluster of the entry
        return entry->firstCluster();
    }
    return 0;
}

void readKernelFile(FATBPB& bpb, uint32 kernelFrstClus) {
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

    bootInfo.allocPhysMemStart = KERNEL_LOAD_PHYS_ADDR + loadOffset;
    alignUp(bootInfo.allocPhysMemStart, PAGE_SIZE);
    bootInfo.allocPhysMemEnd = bootInfo.allocPhysMemStart;
}

void loadSegment(PageTable* mainTable, Elf32ProgramHeader* segHeader) {
    if(!segHeader->shouldLoad()) return;

    //The end of the data segment requires special handling for
    //uninitialized data, which the system defines to begin with zero
    //values.
    //There's a condition that the already loaded segment of the file
    //needs to be copied elsewhere, that is, when the last page of
    //the segment has impure data and the segment has uninitialized
    //data that needs to be set to zero.
    //In this circumstance, it's not possible to set the
    //corresponding bytes of the file to zero since they are much
    //likely to be a part of another segment, therefore rewriting
    //the data of those bytes may lead to catastrophy.

    //In order to simplify the mapping operation, it is a good way
    //to always copy despite the size of impure bytes.
    
    //The virtual base address
    uint32 vBase = segHeader->p_vaddr;
    alignDn(vBase, PAGE_SIZE);
    //The file base address
    uint32 fBase = KERNEL_LOAD_PHYS_ADDR + segHeader->p_offset;
    //The physical base address
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    uint32 pBase = bootInfo.allocPhysMemEnd;
    //Calculate preceding and succeeding bytes
    uint32 precBytes = segHeader->p_vaddr - vBase;
    //Fetch file segment size
    uint32 fSegSize = segHeader->p_filesz;
    //Calculate actual physical memory taken by the segment
    uint32 pSegSize = segHeader->p_memsz;
    alignUp(pSegSize, PAGE_SIZE);
    //Alloc memory
    bootInfo.allocPhysMemEnd += pSegSize;
    //Initialize physical address
    memset((byte*)pBase, 0, pSegSize);
    //Copy segment data
    memcpy((byte*)(pBase + precBytes), (byte*)fBase, fSegSize);
    //Map memory
    memMap(
        pBase,
        vBase,
        pSegSize,
        PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
    );
    if(segHeader->p_vaddr < bootInfo.kernelVirtAddr) {
        bootInfo.kernelVirtAddr = segHeader->p_vaddr;
    }
    bootInfo.kernelSize += pSegSize;
}

void memMap(uint32 pStart, uint32 vStart, uint32 size, PageFlag flags) {
    PageTable* mainTable = PageTable::from(bootInfo.CR3);
    uint32 curPhysicalAddr = pStart;
    uint32 curVirtualAddr = vStart;
    for(int i = 0; i < size; i += PAGE_SIZE) {
        uint32 mainTableIdx = curVirtualAddr >> 22;
        uint32 subTableIdx = (curVirtualAddr << 10) >> 22;
        PageTableEntry& subTableEntry = mainTable->at(mainTableIdx);
        if(!subTableEntry.isPresent()) {
            uint32 subTableAddr = allocPageTable();
            subTableEntry = PageTableEntry(
                subTableAddr,
                PageFlag::PRESENT | PageFlag::WRITABLE | PageFlag::USER_ACCESSIBLE
            );
        }
        PageTable* subTable = PageTable::from(subTableEntry);
        subTable->at(subTableIdx) = PageTableEntry(
            curPhysicalAddr,
            flags
        );
        curPhysicalAddr += PAGE_SIZE;
        curVirtualAddr += PAGE_SIZE;
    }
}

uint32 allocPageTable() {
    alignUp(bootInfo.allocPhysMemEnd, PAGE_SIZE);
    uint32 newPageTableAddr = bootInfo.allocPhysMemEnd;
    memset((byte*)newPageTableAddr, 0, PAGE_SIZE);
    bootInfo.allocPhysMemEnd += PAGE_SIZE;
    return newPageTableAddr;
}

void alignUp(uint32& addr, uint32 alignment) {
    addr = (addr + alignment - 1) / alignment * alignment;
}

void alignDn(uint32& addr, uint32 alignment) {
    addr = addr / alignment * alignment;
}