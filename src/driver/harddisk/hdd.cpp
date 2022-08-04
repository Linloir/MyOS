/*** 
 * Author       : Linloir
 * Date         : 2022-08-03 19:53:41
 * LastEditTime : 2022-08-04 10:44:23
 * Description  : 
 */

#include "hdd.h"

bool HDDManager::_readOneSector(uint idx, byte* dst) {
    _secCntPort.write(0x01);    //Read 1 sector
    _lbaLoPort.write((uint8)idx);
    _lbaMidPort.write((uint8)(idx >> 8));
    _lbaHiPort.write((uint8)(idx >> 16));
    _drivePort.write(((uint8)(idx >> 24)) | 0xE0);
    _cmdPort.write(0x20);
    while(true) {
        HDDStatusFlag status = _statusPort.read();
        if(status.contains(HDDStatusFlag::ERR_OCCURRED)) {
            return false;
        }
        if(status.contains(HDDStatusFlag::DATA_READY)) {
            break;
        }
    }
    for(int i = 0; i < 512; i += 2) {
        uint16 tmpDat = _datPort.read();
        *(uint16*)(dst + i) = tmpDat;
    }
    return true;
}

bool HDDManager::_writeOneSector(uint idx, byte* src) {
    _secCntPort.write(0x01);
    _lbaLoPort.write((uint8)idx);
    _lbaMidPort.write((uint8)(idx >> 8));
    _lbaHiPort.write((uint8)(idx >> 16));
    _drivePort.write(((uint8)(idx >> 24)) | 0xE0);
    _cmdPort.write(0x30);
    while(true) {
        HDDStatusFlag status = _statusPort.read();
        if(status.contains(HDDStatusFlag::ERR_OCCURRED)) {
            return false;
        }
        if(status.contains(HDDStatusFlag::DATA_READY)) {
            break;
        }
    }
    for(int i = 0; i < 512; i += 2) {
        uint16 tempDat = *(uint16*)(src + i);
        _datPort.write(tempDat);
    }
    return true;
}

bool HDDManager::readSector(uint idx, uint cnt, byte* dst) {
    uint offset = 0;
    for(uint i = 0; i < cnt; i++) {
        if(!_readOneSector(idx + i, (byte*)(dst + offset))) {
            uint8 err = _errPort.read();
            memcpy(dst, &err, 8);
            return false;
        }
        offset += 512;
    }
    return true;
}

bool HDDManager::writeSector(uint idx, uint cnt, byte* src) {
    uint offset = 0;
    for(uint i = 0; i < cnt; i++) {
        if(!_writeOneSector(idx + i, (byte*)(src + offset))) {
            uint8 err = _errPort.read();
            memcpy(src, &err, 8);
            return false;
        }
        offset += 512;
    }
    return true;
}