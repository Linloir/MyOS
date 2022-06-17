/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:59:36
 * LastEditTime : 2022-06-17 15:07:59
 * Description  : Page management
 */

#include "systemtype.h"
#include "page.h"

#ifndef _PAGING_H_
#define _PAGING_H_

extern "C" uint32 getCR3();
extern "C" void setCR3(uint32 table);

class PageManager {
    private:
        static PageTableEntry* _mapPage(PageTable* pageTable, uint32 virtualAddr, uint32 physicalAddr, PageFlag flags);
        static PageTableEntry* _mapPage(uint32 virtualAddr, uint32 physicalAddr, PageFlag flags);
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
        static PageTableEntry* mapPage(PageTable* pageTable, Page page, Frame* frame, PageFlag flags);
        static PageTableEntry* mapPage(Page page, Frame* frame, PageFlag flags);
        /**
         * Unmap a physical page from a virtual page, actually removes an entry from the page table
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         * 
         * @param virtualAddr the address of virtual page that is to be unmapped
         */
        static void unmapPage(PageTable* pageTable, Page page);
        static void unmapPage(Page page);
        static bool isPageMapped(Page page);
};

#endif
