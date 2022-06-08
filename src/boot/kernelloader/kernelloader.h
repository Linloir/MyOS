/*** 
 * Author       : Linloir
 * Date         : 2022-05-31 21:25:04
 * LastEditTime : 2022-06-08 09:06:05
 * Description  : 
 */

#ifndef _KERNEL_LOADER_H_
#define _KERNEL_LAODER_H_

#include "os_type.h"

#define KERNEL_START_ADDR 0xB0000000    //3GiB
#define KERNEL_START_SECTOR 16
#define KERNEL_SECTOR_COUNT 200

#define FRST_LEVEL_TABLE_ADDR_BOOT 0x201000     //2MiB + 4KiB
#define FRST_LEVEL_TABLE_ADDR_KERNEL 0x202000   //2MiB + 8KiB
#define SCND_LEVEL_TABLE_ADDR 0x200000          //2MiB

extern "C" void kernelLoader();
extern "C" void readSector(int sector, uint32 addr);
extern "C" void enablePaging(uint32 scndLevelTableAddr);
extern "C" void jumpKernel(uint32 esp, uint32 kernel);
void loadKernel();
void initializePaging();

#endif