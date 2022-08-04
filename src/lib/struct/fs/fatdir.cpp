/*** 
 * Author       : Linloir
 * Date         : 2022-07-29 18:24:45
 * LastEditTime : 2022-07-30 19:16:54
 * Description  : 
 */

#include "fatdir.h"

DirectoryFlag::DirectoryFlag() {
    _attr = 0;
}
DirectoryFlag::DirectoryFlag(uint8 flags) {
    _attr = flags;
}
DirectoryFlag& DirectoryFlag::operator=(uint8 flags) {
    _attr = flags;
    return *this;
}
DirectoryFlag& DirectoryFlag::operator=(const DirectoryFlag& other) {
    _attr = other._attr;
    return *this;
}
//Judge whether two flags are equal
bool DirectoryFlag::operator==(uint8 flags) {
    return _attr == flags;
}
//Combine two flags
DirectoryFlag DirectoryFlag::operator+(uint8 flags) {
    uint8 newAttr = _attr | flags;
    return DirectoryFlag(newAttr);
}
DirectoryFlag& DirectoryFlag::operator+=(uint8 flags) {
    _attr |= flags;
    return *this;
}
//Remove flags
DirectoryFlag DirectoryFlag::operator-(uint8 flags) {
    uint8 newAttr = _attr & ~(flags);
    return DirectoryFlag(newAttr);
}
DirectoryFlag& DirectoryFlag::operator-=(uint8 flags) {
    _attr &= ~(flags);
    return *this;
}
//Bitwise or
DirectoryFlag DirectoryFlag::operator|(uint8 flags) {
    uint8 newAttr = _attr | flags;
    return DirectoryFlag(newAttr);
}
DirectoryFlag& DirectoryFlag::operator|=(uint8 flags) {
    _attr |= flags;
    return *this;
}
//Bitwise and
DirectoryFlag DirectoryFlag::operator&(uint8 flags) {
    uint8 newAttr = _attr & flags;
    return DirectoryFlag(newAttr);
}
DirectoryFlag& DirectoryFlag::operator&=(uint8 flags) {
    _attr &= flags;
    return *this;
}
//Reverse flags
DirectoryFlag DirectoryFlag::operator~() {
    return DirectoryFlag(_attr);
}
//Toggle flag bits
DirectoryFlag& DirectoryFlag::operator^=(uint8 flags) {
    _attr ^= flags;
    return *this;
}
//Judge whether flags are contained
bool DirectoryFlag::contains(uint8 flags) {
    return (_attr & flags) == flags;
}

//Check whether the entry is empty
//@return {bool} : True for the entry IS empty while False for the opposite
bool DirEntry::isEmpty() {
    //According to Microsoft FAT specification:
    //DIR_Name[0] = 0xE5 indicates the directory entry is free
    //DIR_Name[0] = 0x00 indicates the directory entry and all directory entries following the current free entry are free
    //Since 0xE5 would never appear in LFN entries, therefore 0xE5 and 0x00 are unique signatures for empty entries
    return _data[0] == 0xE5 || _data[0] == 0x00;
}

//Check whether the entry is a short filename entry
//@return {bool} : True for the entry IS a short filename entry while False for the opposite
bool DirEntry::isSFNEntry() {
    //According to Microsoft FAT specification:
    //DIR_Attr field indicates whether the entry is a long filename entry
    //READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID indicates a long filename entry
    return !attributes().contains(
        DirectoryFlag::READ_ONLY | 
        DirectoryFlag::HIDDEN | 
        DirectoryFlag::SYSTEM | 
        DirectoryFlag::VOLUME_ID
    );
}

//Check whether the entry is a long filename entry
//@return {bool} : True for the entry IS a long filename entry while False for the opposite
bool DirEntry::isLFNEntry() {
    //According to Microsoft FAT specification:
    //DIR_Attr field indicates whether the entry is a long filename entry
    //READ_ONLY | HIDDEN | SYSTEM | VOLUME_ID indicates a long filename entry
    return attributes().contains(
        DirectoryFlag::READ_ONLY |
        DirectoryFlag::HIDDEN |
        DirectoryFlag::SYSTEM |
        DirectoryFlag::VOLUME_ID
    );
}

//Check whether the entry is the last member in the lfn entry set
//@return {bool} : True for the entry IS the last member in the set while False for the opposite
bool DirEntry::isLastInSet() {
    //According to Microsoft FAT specification:
    //The last long directory name entry in the set is masked with 0x40 in the LDIR_Ord field
    return (_data[0] & LAST_LONG_ENTRY) != 0;
}

//Return the DIR_Attr field in the entry
//@return {DirectoryFlag} : The attributes of the entry in DirectoryFlag format
DirectoryFlag DirEntry::attributes() {
    //According to Microsoft FAT specification:
    //DIR_Attr field has an offset of 11 and size of 1 byte
    return DirectoryFlag(_data[11]);
}

//Return the creation time of the file/directory in FATTime format
//It should be noted that only SFN entries hold this field
//@return {FATTime} : The creation time, including hour, minute, second and millisecond
FATTime DirEntry::createTime() {
    //According to Microsoft FAT specification:
    //DIR_CrtTimeTenth has an offset of 13 and size of 1 byte
    //DIR_CrtTime has an offset of 14 and size of 2 bytes
    return FATTime(_data[13], *(uint16*)(&_data[14]));
}

//Return the creation date of the file/directory in FATDate format
//It should be noted that only SFN entries hold this field
//@return {FATDate} : The creation date, including year, month and day
FATDate DirEntry::createDate() {
    //According to Microsoft FAT specification
    //DIR_CrtDate has an offset of 16 and size of 2 bytes
    return FATDate(*(uint16*)(&_data[16]));
}

//Return the last accessed date of the file/directory in FATDate format
//It should be noted that only SFN entries hold this field
//@return {FATDate} : The last accessed date, including year, month and day
FATDate DirEntry::lastAccessedDate() {
    //According to Microsoft FAT specification
    //DIR_LstAccDate has an offset of 18 and size of 2 bytes
    return FATDate(*(uint16*)(&_data[18]));
}

//Return the last modification time of the file/directory in FATTime format
//It should be noted that only SFN entries hold this field
//@return {FATTime} : The last modification time, including hour, minute, second and millisecond
FATTime DirEntry::lastModifiedTime() {
    //According to Microsoft FAT specification
    //Last modification time is stored in DIR_WrtTime field
    //DIR_WrtTime has an offset of 22 and size of 2 bytes
    //It should be noted that last modification time does not provide granularity under 2 seconds
    //More specifically, there is no timetenth field for last modification time
    return FATTime(0, *(uint16*)(&_data[22]));
}

//Return the last modification date of the file/directory in FATDate format
//It should be noted that only SFN entries hold this field
//@return {FATDate} : The last modification date, including year, month and day
FATDate DirEntry::lastModifiedDate() {
    //According to Microsoft FAT specification
    //Last modification date is stored in DIR_WrtDate field
    //DIR_WrtDate has an offset of 24 and size of 2 bytes
    return FATDate(*(uint16*)(&_data[24]));
}

//Return the first cluster of the file/directory
//It should be noted that only SFN entries hold this field
//@return {uint32} : The first cluster of the file/directory
uint32 DirEntry::firstCluster() {
    //According to Microsoft FAT specification
    //first cluster is defined in DIR_FstClusHI and DIR_FstClusLO
    //This function simply concatenates the high and low bits and returns
    uint16 frstClusterHI = *(uint16*)(&_data[20]);
    uint16 frstClusterLO = *(uint16*)(&_data[26]);
    uint32 frstCluster = ((uint32)frstClusterHI << 16) | frstClusterLO;
    return frstCluster;
}

//Return the size of the file/directory
//It should be noted that only SFN entries hold this field
//@return {uint32} : The size of the file/directory in bytes
uint32 DirEntry::fileSize() {
    //According to Microsoft FAT specification
    //The DIR_FileSize field holds the 32-bit quantity containing size in bytes of file/directory described by this entry
    //DIR_FileSize field has an offset of 28 and size of 4 bytes
    return *(uint32*)(&_data[28]);
}

//Return the order of the entry in the set of LFN entries
//It should be noted that only LFN entries hold this field
//@return {uint32} : The order of the entry
uint32 DirEntry::lfnIndex() {
    //According to Microsoft FAT specification:
    //The order of the entry is defined in LDIR_Ord field
    //LDIR_Ord field has an offset of 0 and size of 1 byte
    //
    //It should also be noted that the field can be masked with 0x40 if it's the last entry in the set
    //
    //Therefore, bit 6 should be masked out during returing
    //
    //A pretty safe way is to return only the low 6 bits of this field
    //since the long filename would not exceed the 255 characters limit,
    //which equals to 255/13 = 20 = 0x14 entries, of which bit 5 is not even used for maximum length filename
    return _data[0] & 0x3F;
}

//Extract every first bit out of two bits of the filename
//transforming the filename into ASCII format and copy the transformed filename to dst
//It should be noted that this function BEHAVE DIFFERENTLY in SFN and LFN entries
//- in LFN entries: the filename is transformed into ASCII and copied to param dst
//- in SFN entries: the short filename is copied to param dst
//@param {byte*} dst : Destination memory where the filename will be copied to
void DirEntry::parseASCIIFileName(byte* dst) {
    //According to Microsoft FAT specification:
    if(isLFNEntry()) {
        //The filename in LFN entry is stored in three parts
        //LDIR_Name1 has an offset of 1 and size of 10 bytes
        //LDIR_Name2 has an offset of 14 and size of 12 bytes
        //LDIR_Name3 has an offset of 28 and size of 4 bytes
        //The file name is stored in UNICODE which is 16-bit per character
        //
        //Since only English words is used in the implementation
        //it's okay to extract every frist bit out of two to transform UNICODE into ASCII
        for(int i = 0; i < 5; i++) {
            dst[i] = _data[1 + 2 * i];
        }
        for(int i = 0; i < 6; i++) {
            dst[5 + i] = _data[14 + 2 * i];
        }
        for(int i = 0; i < 2; i++) {
            dst[11 + i] = _data[28 + 2 * i];
        }
    }
    else {
        //The filename in SFN entry is sotred in DIR_Name field
        //which as an offset of 0 and size of 11 bytes
        for(int i = 0; i < 11; i++) {
            dst[i] = _data[i];
        }
    }
}

//Calculate/Return the checksum of the entry
//It should be noted that this function BEHAVE DIFFERENTLY in SFN and LFN entries
//@return {uint32} : Calculate checksum out of filename in SFN entries
//@return {uint32} : Return checksum in LFN entries
uint32 DirEntry::checkSum() {
    if(isSFNEntry()) {
        //ChkSum()
        //Returns an unsigned byte checksum computed on an unsigned byte
        //array. The array must be 11 bytes long and is assumed to contain
        //a name stored in the format of a MS-DOS directory entry

        //The following method is copied from Microsoft FAT specification
        //page 32, chapter 7.2 Checksum generation
        byte* pName = &_data[0];
        uint16 nameLen;
        uint8 sum;
        sum = 0;
        for(nameLen = 11; nameLen != 0; nameLen--) {
            //NOTE: The operation is an unsigned char rotate right
            sum = ((sum & 1) ? 0x80 : 0) + (sum >> 1) + *pName++;
        }
        return sum;
    }
    else {
        //According to Microsoft FAT specification:
        //The checksum in LFN entries is stored in LDIR_Chksum field
        //which has an offset of 13 and size of 1 byte
        return _data[13];
    }
}