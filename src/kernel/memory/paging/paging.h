/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:59:36
 * LastEditTime : 2022-06-03 22:47:33
 * Description  : Page management
 */

#ifndef _PAGING_H_
#define _PAGING_H_

#include "os_type.h"
#include "page.h"

extern "C" uint32 getCR3();
extern "C" void setCR3(uint32 table);

class PageManager {
    public:
        static void initialize();
        /**
         * Map a virtual page to a physical page, actually adds an entry into the page table
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         *
         * @param physicalAddr the address of physical page that is mapped to
         * @param virtualAddr the address of virtual page that is mapped from
         * @param flags the flags of the mapping page
         */
        static uint32 mapPage(uint32 virtualAddr, uint32 physicalAddr, PageFlag flags);
        /**
         * Unmap a physical page from a virtual page, actually removes an entry from the page table
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         * 
         * @param virtualAddr the address of virtual page that is to be unmapped
         */
        static uint32 unmapPage(uint32 virtualAddr);
};

#endif
