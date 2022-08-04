/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 18:18:24
 * LastEditTime : 2022-07-31 17:56:19
 * Description  : BIOS Parameter Block
 */

#include "types.h"
#include "fatfsinfo.h"

#ifndef _FAT_BPB_H_
#define _FAT_BPB_H_

class FATBPB {
    private:
        byte _data[512];
    public:
        FATBPB(uint32 bpbFrstSec);
    public:
        //Metadata parsing
        uint16 bytesPerSector();
        uint8 sectorsPerCluster();
        uint16 sectorsPerTrack();
        uint16 headCount();
        uint16 reservedSectorCount();
        uint32 hiddenSectorCount();
        uint32 totalSectors();
        uint8 fatCount();
        uint32 fatSize();
        uint8 activeFAT();      //FAT32 only
        bool isFATMirrored();       //FAT32only
        uint16 rootEntryCount();//FAT16 only
        uint32 rootCluster();   //FAT32 only
        char* oemName();
        uint8 mediaType();
        uint8 driveNumber();
        bool hasExtendedBootSignature();
        uint32 volumeSerialNumber();
        char* volumeLabel();
        char* fsTypeLabel();
        uint16 fsInfoSector();  //FAT32 only
        //Basic calculation
        bool isFAT12();
        bool isFAT16();
        bool isFAT32();
        uint32 fatEntrySize();
        uint32 fatFrstSec();
        uint32 fatFrstSecAbsolute();
        uint32 fatTotalSec();
        uint32 rootDirFrstSec();
        uint32 rootDirFrstSecAbsolute();
        uint32 rootDirTotalSec();
        uint32 dataFrstSec();
        uint32 dataFrstSecAbsolute();
        uint32 dataTotalSec();
        uint32 totalClusters();
        uint32 secOfCluster(uint32 clus);
        uint32 absoluteSecOfCluster(uint32 clus);
};

#endif