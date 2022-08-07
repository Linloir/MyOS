/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 22:09:47
 * LastEditTime : 2022-08-07 21:12:16
 * Description  : 
 */

#include "partition.h"
#include "hdd.h"
#include "mbr.h"
#include "fatbpb.h"
#include "fatdir.h"
#include "../../lib/struct/elf/elf.h"
#include "str.h"
#include "pgtable.h"
#include "memory.h"
#include "constants.h"
#include "bootinfo.h"

#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#define KERNEL_LOAD_PHYS_ADDR 0x100000

extern "C" void kernelLoader();

void readKernel();
void initPageTable();
void loadKernel();
void allocPages();
void enablePaging();
void boot();

void readFromPartition(Partition p);
uint32 locateSysFolder(uint32 rootSector);
uint32 locateKernelFile(uint32 systemDirSec);
void readKernelFile(FATBPB& bpb, uint32 kernelFrstClus);

void loadSegment(PageTable* mainTable, Elf32ProgramHeader* segHeader);

void memMap(uint32 pStart, uint32 vStart, uint32 size, PageFlag flags);
uint32 allocPageTable();

void alignUp(uint32& addr, uint32 alignment);
void alignDn(uint32& addr, uint32 alignment);

#endif