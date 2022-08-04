/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 22:09:47
 * LastEditTime : 2022-08-03 18:35:01
 * Description  : 
 */

#include "partition.h"
#include "hdd.h"
#include "mbr.h"
#include "fatbpb.h"
#include "fatdir.h"
#include "str.h"
#include "pgtable.h"
#include "memory.h"

#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LOADER_H_

#define KERNEL_LOAD_PHYS_ADDR 0x100000

extern "C" void kernelLoader();

uint32 readKernel();
PageTable* enablePaging();
void loadKernel(PageTable* pgtable, uint32 kernelSize);
void boot();

uint32 readFromPartition(Partition p);
uint32 locateSysFolder(uint32 rootSector);
uint32 locateKernelFile(uint32 systemDirSec);
uint32 readKernelFile(FATBPB& bpb, uint32 kernelFrstClus);

#endif