/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 21:48:29
 * LastEditTime : 2022-05-31 18:05:57
 * Description  : Bit Map
 */

#include "bitmap.h"
#include "allocator.h"

int BitMap::firstAvailable(int start) {
    if(!freeSize) {
        return -1;
    }

    int index = start >> 3;
    int offset = start & 3;
    int curBlock = map[index] >> offset;

    for(int i = start; i < size; i++) {
        bool status = curBlock & 1;
        if(status) {
            return i;
        }
        if(offset == 7) {
            offset = 0;
            index++;
            curBlock = map[index];
            continue;
        }
        offset++;
        curBlock >>= 1;
    }
    
    return -1;
}

BitMap::BitMap(int length) {
    map = (uint8*)malloc(sizeof(uint8) * length);
    for(int i = 0; i < length; i++) {
        map[i] = 0;
    }
    size = length;
    freeSize = length;
}

int BitMap::availableResources() {
    return freeSize;
}

int BitMap::unavailableResources() {
    return size - freeSize;
}

int BitMap::totalResources() {
    return size;
}

void BitMap::set(int index, bool status) {
    map[index >> 3] &= (0 << (index & 3));
    map[index >> 3] &= ((status ? 1 : 0) << (index & 3));
    if(status) {
        freeSize--;
    }
}

int BitMap::alloc() {
    int index = firstAvailable(0);
    if(index == -1) {
        return -1;
    }
    set(index, true);
    freeSize--;
    return index;
}

void BitMap::free(int index) {
    set(index, false);
    freeSize++;
}