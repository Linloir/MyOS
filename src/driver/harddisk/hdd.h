/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 23:06:14
 * LastEditTime : 2022-08-03 19:53:57
 * Description  : Hard disk driver
 */

#include "types.h"
#include "memory.h"
#include "ports.h"

#ifndef _HDD_H_
#define _HDD_H_

class HDDStatusFlag {
    public:
        enum ATTR {
            ERR_OCCURRED            = 1,
            IDX                     = 1 << 1,
            CORR                    = 1 << 2,
            DATA_READY              = 1 << 3,
            OVERLAPP_SRVICE_REQUEST = 1 << 4,
            DRIVE_FAULT_ERR         = 1 << 5,
            DRIVE_READY             = 1 << 6,
            BUSY                    = 1 << 7
        };
        HDDStatusFlag() {}
        HDDStatusFlag(byte flags) {
            _attr = static_cast<byte>(flags);
        }
        HDDStatusFlag& operator=(byte flags) {
            _attr = static_cast<byte>(flags);
            return *this;
        }
        HDDStatusFlag& operator=(const HDDStatusFlag& other) {
            _attr = other._attr;
            return *this;
        }
        //Judge whether two flags are equal
        bool operator==(byte flags) {
            return _attr == static_cast<byte>(flags);
        }
        //Combine two flags
        HDDStatusFlag operator+(byte flags) {
            byte newAttr = _attr | static_cast<byte>(flags);
            return HDDStatusFlag(static_cast<byte>(newAttr));
        }
        HDDStatusFlag& operator+=(byte flags) {
            _attr |= static_cast<byte>(flags);
            return *this;
        }
        //Remove flags
        HDDStatusFlag operator-(byte flags) {
            byte newAttr = _attr & ~(static_cast<byte>(flags));
            return HDDStatusFlag(static_cast<byte>(newAttr));
        }
        HDDStatusFlag& operator-=(byte flags) {
            _attr &= ~(static_cast<byte>(flags));
            return *this;
        }
        //Bitwise or
        HDDStatusFlag operator|(byte flags) {
            byte newAttr = _attr | static_cast<byte>(flags);
            return HDDStatusFlag(static_cast<byte>(newAttr));
        }
        HDDStatusFlag& operator|=(byte flags) {
            _attr |= static_cast<byte>(flags);
            return *this;
        }
        //Bitwise and
        HDDStatusFlag operator&(byte flags) {
            byte newAttr = _attr & static_cast<byte>(flags);
            return HDDStatusFlag(static_cast<byte>(newAttr));
        }
        HDDStatusFlag& operator&=(byte flags) {
            _attr &= static_cast<byte>(flags);
            return *this;
        }
        //Reverse flags
        HDDStatusFlag operator~() {
            return HDDStatusFlag(static_cast<byte>(_attr));
        }
        //Toggle flag bits
        HDDStatusFlag& operator^=(byte flags) {
            _attr ^= static_cast<byte>(flags);
            return *this;
        }
        //Judge whether flags are contained
        bool contains(byte flags) {
            return (_attr & static_cast<byte>(flags)) == static_cast<byte>(flags);
        }
    private:
        byte _attr;
};

class HDDManager {
    public:
        HDDManager() {}
    private:
        const Port<uint16> _datPort = 0x1F0;
        const Port<uint8> _errPort = Port<uint8>(0x1F1, Port<uint8>::READ);
        const Port<uint8> _funcPort = Port<uint8>(0x1F1, Port<uint8>::WRITE);
        const Port<uint8> _secCntPort = 0x1F2;
        const Port<uint8> _lbaLoPort = 0x1F3;
        const Port<uint8> _lbaMidPort = 0x1F4;
        const Port<uint8> _lbaHiPort = 0x1F5;
        const Port<uint8> _drivePort = 0x1F6;
        const Port<uint8> _statusPort = Port<uint8>(0x1F7, Port<uint8>::READ);
        const Port<uint8> _cmdPort = Port<uint8>(0x1F7, Port<uint8>::WRITE);
        bool _readOneSector(uint idx, byte* dst);
        bool _writeOneSector(uint idx, byte* src);
    public:
        bool readSector(uint idx, uint cnt, byte* dst);
        bool writeSector(uint idx, uint cnt, byte* src);
};

#endif
