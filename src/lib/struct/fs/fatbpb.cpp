/*** 
 * Author       : Linloir
 * Date         : 2022-07-29 17:57:05
 * LastEditTime : 2022-08-04 10:17:33
 * Description  : BPB class
 */

#include "fatbpb.h"
#include "hdd.h"

//Read the first sector of the volume into buffer
//@param {uint32} bpbFrstSec : The first sector of the bpb region
FATBPB::FATBPB(uint32 bpbFrstSec) {
    HDDManager().readSector(bpbFrstSec, 1, _data);
}

//Returns count of bytes per sector of the disk
//@return {uint16} : following values: 512, 1024, 2048 or 4096
uint16 FATBPB::bytesPerSector() {
    return *(uint16*)(&_data[11]);
}

//Returns number of sectors per allocation unit
//@return {uint8} : A power of 2 that is greater than 0.
//                  Legal values are 1, 2, 4, 8, 16, 32, 64 and 128
uint8 FATBPB::sectorsPerCluster() {
    return _data[13];
}

//Returns sectors per track for interrupt 0x13.
//This is only relevant for media that have a geometry and are visible
//on interrupt 0x13
//@return {uint16} : sectors per track of the media
uint16 FATBPB::sectorsPerTrack() {
    return *(uint16*)(&_data[24]);
}

//Returns the number of heads of the media for interrupt 0x13.
//This is only relevant as discussed earlier for [sectorsPerTrack()]
//@return {uint32} : One based number for number of heads
//                   For example, on a 1.44MB 3.5-inch floppy drive this
//                   value is 2
uint16 FATBPB::headCount() {
    return *(uint16*)(&_data[26]);
}

//Returns the number of reserved sectors in the reserved region of the volume
//starting at the first sector of the volume, which is also the size of bios 
//parameter block in sectors
//@return {uint16} : Non-zero value of the number of reserved sectors
uint16 FATBPB::reservedSectorCount() {
    return *(uint16*)(&_data[14]);
}

//Returns the count of hidden sectors preceding the partition that contains
//this FAT volume. This method is generally only relevant for media visible
//on interrupt 0x13
//@return {uint32} : The sectors preceding the partition
uint32 FATBPB::hiddenSectorCount() {
    return *(uint32*)(&_data[28]);
}

//Returns the 32-bit total count of sectors on the volume. The count includes
//the count of all sectors in all four regions of the volume
//@return {uint32} : The total count of sectors
uint32 FATBPB::totalSectors() {
    //It should be noted that for volumes that contain more than or equal to 0x10000
    //sectors, BPB_TotalSec32 field is used to store the total sectors count. In other
    //circumstances, BPB_TotalSec16 field is used to store the total sectors count
    //
    //In addition, BPB_TotalSec32 field should only be considered valid if BPB_TotalSec16
    //is zero
    return *(uint16*)(&_data[19]) == 0 ? *(uint32*)(&_data[32]) : (uint32)(*(uint16*)(&_data[19]));
}

//Returns the count of file allocation tables (FATs) on the volume
//@return {uint8} : The count of FATs. A value of 2 is recommended although 1 is acceptable
uint8 FATBPB::fatCount() {
    return _data[16];
}

//Returns the 32-bit count of sectors occupied by one FAT
//@return {uint32} : The sectors occupied by one FAT
uint32 FATBPB::fatSize() {
    //It should be noted that for FAT12/FAT16 volumes the sectors occupied by one FAT
    //is stored in 16-bit BPB_FATSz16 field while for FAT32 volumes the value is stored
    //in 32-bit BPB_FATSz32 field.
    uint32 fatSz16 = (uint32)(*(uint16*)(&_data[22]));
    if(fatSz16 != 0) {
        return fatSz16;
    }
    else {
        return *(uint32*)(&_data[36]);
    }
}

//Returns the active FAT on the volume
//It should be noted that this method is FAT32 only
//@return {uint8} : Zero-based number of active FAT, only valid if mirroring is disabled
uint8 FATBPB::activeFAT() {
    return _data[40] & 0xF;
}

//Returns whether mirroring is disabled on the volume
//It should be noted that this method is FAT32 only
//@return {bool} : True means the FAT is morrored at runtime into all FATs.
//                 False means only one FAT is active, it is the one referenced by
//                 [activeFAT()]
bool FATBPB::isFATMirrored() {
    return (_data[40] & 0x80) != 0;
}

//Returns the count of 32-byte directory entries in the root directory
//@return {uint16} : The count of directory entries in the root directory.
//                   This value should always specify a count that when multiplied
//                   by 32 results in an even multiple of [bytesPerSecter()]
uint16 FATBPB::rootEntryCount() {
    return *(uint16*)(&_data[17]);
}

//Returns the cluster number of the first cluster of the root directory
//It should be noted that this method is FAT32 only
//@return {uint32} : the root cluster number
uint32 FATBPB::rootCluster() {
    return *(uint32*)(&_data[44]);
}

//Returns the OEM name identifier in the bios parameter block
//@return {char*} : The pointer to the field containing the OEM name in the buffer
char* FATBPB::oemName() {
    return (char*)(&_data[3]);
}

//Returns the media type
//@return {uint8} : Legal values are 0xF0, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE and 0xFF
//                  0xF8 is standard value for fixed media while 0xF0 is for removable media
uint8 FATBPB::mediaType() {
    return _data[21];
}

//Returns the drive number for interrupt 0x13
//@return {uint8} : 0x80 or 0x00
uint8 FATBPB::driveNumber() {
    //It should be noted that the field containing the value has different offset on FAT12/16
    //volumes and FAT32 volumes
    //On FAT12/16 volumes the field has an offset of 36 while on FAT32 volumes has an offset
    //of 64
    if(isFAT32()) {
        return _data[64];
    }
    else {
        return _data[36];
    }
}

//Check whether the volume contains the extended boot signature which indicates
//the BS_VolOD, BS_VolLab and BS_FilSysType fields are present
//@return {bool} : Ture for the extended boot signature value is set to 0x29
bool FATBPB::hasExtendedBootSignature() {
    //It should be noted that the field containing the value has different offset on FAT12/16
    //volumes and FAT32 volumes
    //On FAT12/16 volumes the field has an offset of 38 while on FAT32 volumes has an offset
    //of 66
    if(isFAT32()) {
        return _data[66] == 0x29;
    }
    else {
        return _data[38] == 0x29;
    }
}

//Returns the volume serial number for volume tracking on removable media.
//It should be noted that this value is only present when [hasExtendedBootSignature()]
//function returns a true value.
//@return {uint32} : Volume serial number
uint32 FATBPB::volumeSerialNumber() {
    //It should be noted that the field containing the value has different offset on FAT12/16
    //volumes and FAT32 volumes
    //On FAT12/16 volumes the field has an offset of 39 while on FAT32 volumes has an offset
    //of 67
    if(isFAT32()) {
        return *(uint32*)(&_data[67]);
    }
    else {
        return *(uint32*)(&_data[39]);
    }
}

//Returns the volume label
//@return {char*} : The pointer of the volume label stored in the buffer
char* FATBPB::volumeLabel() {
    //It should be noted that the field containing the value has different offset on FAT12/16
    //volumes and FAT32 volumes
    //On FAT12/16 volumes the field has an offset of 43 while on FAT32 volumes has an offset
    //of 71
    if(isFAT32()) {
        return (char*)(&_data[71]);
    }
    else {
        return (char*)(&_data[43]);
    }
}

//Returns the file system type string
//@return {char*} : The pointer of the fs type string, the string should be one of the
//                  strings: "FAT", "FAT12", "FAT16" or "FAT32"
char* FATBPB::fsTypeLabel() {
    //It should be noted that the field containing the value has different offset on FAT12/16
    //volumes and FAT32 volumes
    //On FAT12/16 volumes the field has an offset of 54 while on FAT32 volumes has an offset
    //of 82
    if(isFAT32()) {
        return (char*)(&_data[82]);
    }
    else {
        return (char*)(&_data[54]);
    }
}

//Returns the sector number of FSINFO structure in the reserved area of the FAT32 volume.
//It should be noted that this method is only available on FAT32 volumes.
//@return {uint16} : FSINFO structure sector number, usually 1
uint16 FATBPB::fsInfoSector() {
    return *(uint16*)(&_data[48]);
}

//Check whether the volume is FAT12
bool FATBPB::isFAT12() {
    uint32 totClus = totalClusters();
    return totClus < 4085;
}

//Check whether the volume is FAT16
bool FATBPB::isFAT16() {
    uint32 totClus = totalClusters();
    return totClus >= 4085 && totClus < 65525;
}

//Check whether the volume is FAT32
bool FATBPB::isFAT32() {
    uint32 totClus = totalClusters();
    return totClus >= 65525;
}

//Return the size in bytes of a fat entry
//@return {uint32} : 12 for FAT12, 16 for FAT16, 32 for FAT32
uint32 FATBPB::fatEntrySize() {
    uint32 totClus = totalClusters();
    if(totClus < 4085) {
        return 12;
    }
    else if(totClus < 65525) {
        return 16;
    }
    else {
        return 32;
    }
}

//Returns the first sector of first FAT
uint32 FATBPB::fatFrstSec() {
    return reservedSectorCount();
}

//Returns the ABSOLUTE sector number of first FAT
uint32 FATBPB::fatFrstSecAbsolute() {
    return hiddenSectorCount() + reservedSectorCount();
}

//Returns the total sectors of FATs
uint32 FATBPB::fatTotalSec() {
    return fatCount() * fatSize();
}

//Returns the first sector of the root directory region
uint32 FATBPB::rootDirFrstSec() {
    uint32 resvdSecCnt = reservedSectorCount();
    uint32 fatSecCnt = fatTotalSec();
    return resvdSecCnt + fatSecCnt;
}

//Returns the ABSOLUTE sector number of the first sector of the root
//directory region
uint32 FATBPB::rootDirFrstSecAbsolute() {
    uint32 resvdSecCnt = reservedSectorCount();
    uint32 fatSecCnt = fatTotalSec();
    uint32 hiddenSec = hiddenSectorCount();
    return hiddenSec + resvdSecCnt + fatSecCnt;
}

//Returns the total sectors of root directory region
uint32 FATBPB::rootDirTotalSec() {
    uint32 rootEntCnt = rootEntryCount();
    uint32 bytsPerSec = bytesPerSector();
    return (rootEntCnt * 32 + (bytsPerSec - 1)) / bytsPerSec;
}

//Returns the first sector of data region
uint32 FATBPB::dataFrstSec() {
    uint32 resvdSecCnt = reservedSectorCount();
    uint32 fatSecCnt = fatTotalSec();
    uint32 rootSecCnt = rootDirTotalSec();
    return resvdSecCnt + fatSecCnt + rootSecCnt;
}

//Returns the ABSOLUTE sector number of data region
uint32 FATBPB::dataFrstSecAbsolute() {
    uint32 resvdSecCnt = reservedSectorCount();
    uint32 fatSecCnt = fatTotalSec();
    uint32 rootSecCnt = rootDirTotalSec();
    uint32 hiddenSec = hiddenSectorCount();
    return hiddenSec + resvdSecCnt + fatSecCnt + rootSecCnt;
}

//Returns the total sectors of data region
uint32 FATBPB::dataTotalSec() {
    uint32 totSec = totalSectors();
    uint32 resvdSecCnt = reservedSectorCount();
    uint32 fatSecCnt = fatTotalSec();
    uint32 rootSecCnt = rootDirTotalSec();
    return totSec - (resvdSecCnt + fatSecCnt + rootSecCnt);
}

//Returns the total clusters of data region
uint32 FATBPB::totalClusters() {
    return dataTotalSec() / sectorsPerCluster();
}

//Returns the sector of the given cluster
//@param {uint32} clus : The zero-based cluster number
//@return {uint32} : The sector number of the cluster
uint32 FATBPB::secOfCluster(uint32 clus) {
    uint32 frstDataSec = dataFrstSec();
    uint32 clusterOffset = clus - 2;
    uint32 secPerClus = sectorsPerCluster();
    return frstDataSec + clusterOffset * secPerClus;
}

//Returns the ABSOLUTE sector of the given cluster
//@param {uint32} clus : The zero-based cluster number
//@return {uint32} : The absolute sector number of the cluster
uint32 FATBPB::absoluteSecOfCluster(uint32 clus) {
    uint32 frstDataSec = dataFrstSec();
    uint32 clusterOffset = clus - 2;
    uint32 secPerClus = sectorsPerCluster();
    uint32 hiddenSec = hiddenSectorCount();
    return hiddenSec + frstDataSec + clusterOffset * secPerClus;
}