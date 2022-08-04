/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 18:20:03
 * LastEditTime : 2022-07-30 18:19:57
 * Description  : FAT directory entry
 */

#include "types.h"
#include "memory.h"
#include "fatdt.h"

#ifndef _FAT_DIR_H_
#define _FAT_DIR_H_

#define LAST_LONG_ENTRY 0x40

class DirectoryFlag;
class DirEntry;

class RAWEntry;
class SFNEntry;
class LFNEntry;

class DirectoryFlag {
    public:
        enum ATTRIBUTES {
            READ_ONLY   = 1,
            HIDDEN      = 1 << 1,
            SYSTEM      = 1 << 2,
            VOLUME_ID   = 1 << 3,
            DIRECTORY   = 1 << 4,
            ARCHIVE     = 1 << 5
        };
    private:
        uint8 _attr;
    public:
        DirectoryFlag();
        DirectoryFlag(uint8 flags);
    public:
        DirectoryFlag& operator=(uint8 flags);
        DirectoryFlag& operator=(const DirectoryFlag& other);
        //Judge whether two flags are equal
        bool operator==(uint8 flags);
        //Combine two flags
        DirectoryFlag operator+(uint8 flags);
        DirectoryFlag& operator+=(uint8 flags);
        //Remove flags
        DirectoryFlag operator-(uint8 flags);
        DirectoryFlag& operator-=(uint8 flags);
        //Bitwise or
        DirectoryFlag operator|(uint8 flags);
        DirectoryFlag& operator|=(uint8 flags);
        //Bitwise and
        DirectoryFlag operator&(uint8 flags);
        DirectoryFlag& operator&=(uint8 flags);
        //Reverse flags
        DirectoryFlag operator~();
        //Toggle flag bits
        DirectoryFlag& operator^=(uint8 flags);
        //Judge whether flags are contained
        bool contains(uint8 flags);
};

class DirEntry {
    private:
        byte _data[32];
    public:
        bool isEmpty();
        bool isSFNEntry();
        bool isLFNEntry();
        bool isLastInSet();
        DirectoryFlag attributes();
        FATTime createTime();
        FATDate createDate();
        FATDate lastAccessedDate();
        FATTime lastModifiedTime();
        FATDate lastModifiedDate();
        uint32 firstCluster();
        uint32 fileSize();
        uint32 lfnIndex();
        void parseASCIIFileName(byte* dst);
        uint32 checkSum();
};

#endif