/*** 
 * Author       : Linloir
 * Date         : 2022-06-06 16:06:18
 * LastEditTime : 2022-06-07 17:37:14
 * Description  : 
 */

#ifndef _SWAP_H_
#define _SWAP_H_

#include "os_type.h"
#include "vector.h"
#include "frame.h"
#include "page.h"

struct SwapRecord {
    private:
        uint32 val;
    public:
        SwapRecord(uint32 sector, PageFlag oldFlags);
        uint32 sector();
        PageFlag pageFlags();
        PageTableEntry* toPageTableEntry(uint32 newAddress);
};

class SwapManager {
    private:
        static Vec<uint32> _freeSectors;
    public:
        static void initialize(uint32 totalSectors);
        static void swapOut(Frame frame);
        static void swapIn(SwapRecord* record); 
};

extern "C" void readSector(uint32 sector, uint32 addr);
extern "C" void writeSector(uint32 sector, uint32 addr);

#endif