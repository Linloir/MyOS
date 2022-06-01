/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:43:15
 * LastEditTime : 2022-05-31 21:27:04
 * Description  : Paging
 */

#include "paging.h"

uint32 PageManager::ID_MAPPED_TABLE_ADDR = 0;

void PageManager::initialize(uint32 scndLevTableAddr, uint32 mappedTableAddr) {
    ID_MAPPED_TABLE_ADDR = mappedTableAddr;
    asm_set_cr3(scndLevTableAddr);
}

void PageManager::setMappedTableAddr(uint32 addr) {
    ID_MAPPED_TABLE_ADDR = addr;
}



void PageManager::mapPage(uint32 physicalAddr, uint32 virtualAddr, PagePermissionFlag permission, PagePriviledgeFlag priviledge) {

}