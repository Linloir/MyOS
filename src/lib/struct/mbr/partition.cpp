/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 10:06:27
 * LastEditTime : 2022-08-04 10:06:28
 * Description  : 
 */

#include "partition.h"

bool Partition::isPresent() {
    return sectorNum() != 0;
}

bool Partition::isActive() {
    return _status == ACTIVE;
}

uint32 Partition::firstSectorLBA() {
    return *(uint32*)_frstSecLBA;
}

uint32 Partition::sectorNum() {
    return *(uint32*)_numSec;
}