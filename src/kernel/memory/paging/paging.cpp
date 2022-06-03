/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:43:15
 * LastEditTime : 2022-06-03 13:34:29
 * Description  : Paging
 */

#include "paging.h"

uint32 PageManager::ID_MAPPED_TABLE_ADDR = 0xB0101000;

void PageManager::setMappedTableAddr(uint32 addr) {
    ID_MAPPED_TABLE_ADDR = addr;
}

void PageManager::mapPage(uint32 physicalAddr, uint32 virtualAddr, PagePermissionFlag permission, PagePriviledgeFlag priviledge) {
    
}