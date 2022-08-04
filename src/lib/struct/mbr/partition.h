/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 21:45:02
 * LastEditTime : 2022-08-04 10:06:22
 * Description  : MBR Disk Partition Table structure
 */

#include "types.h"

#ifndef _PARTITION_H_
#define _PARTITION_H_

class CHSAddress {
    public:
        byte address[3];
    public:
        uint32 cylinder();
        uint32 head();
        uint32 sector();
};

class Partition {
    public:
        enum Status {
            INACTIVE    = 0,
            ACTIVE      = 1 << 7
        };
        enum Type {
            EMPTY           = 0,
            FAT12           = 0x01,
            FAT16           = 0x04,
            FAT32_CHS       = 0x0B,
            FAT32_LBA       = 0x0C,
        };
    public:
        uint8 _status;          //Status for bootable
        CHSAddress _frstSecCHS;  //CHS address of first absolute sector in partition
        uint8 _type;              //Partition type
        CHSAddress _lastSecCHS;  //CHS address of last absolute sector in partition
        byte _frstSecLBA[4];     //LBA address of first absolute sector in partition
        byte _numSec[4];         //Number of sector in the partition
    public:
        bool isPresent();
        bool isActive();
        uint32 firstSectorLBA();
        uint32 sectorNum();
};

#endif