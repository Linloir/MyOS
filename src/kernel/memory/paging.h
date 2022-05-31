/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:59:36
 * LastEditTime : 2022-05-31 11:46:40
 * Description  : Page management
 */

#ifndef _PAGING_H_
#define _PAGING_H_

#include "os_type.h"

class PageManager {
    private:
        uint32* ID_MAPPED_TABLE_ADDR = NULL;    //The identity-mapped level 1 table for temporary mapping
    public:
        PageManager();
        /**
         * Idendity-map a page
         * 
         * @param physicalAddr the physical address of a page to be mapped
         */
        void identityMapPage(uint32 physicalAddr);
        /**
         * Map a virtual page to a physical page
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         *
         * @param physicalAddr the address of physical page that is mapped to
         * @param virtualAddr the address of virtual page that is mapped from
         */
        void mapPage(uint32 physicalAddr, uint32 virtualAddr);
        /**
         * Unmap a physical page from a virtual page
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         * 
         * @param physicalAddr the address of physical page that is to be unmapped
         * @param virtualAddr the address of virtual page that is to be mapped
         */
        void unmapPage(uint32 physicalAddr, uint32 virtualAddr);
}

#endif
