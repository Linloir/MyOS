/*** 
 * Author       : Linloir
 * Date         : 2022-05-30 19:59:36
 * LastEditTime : 2022-06-03 13:32:03
 * Description  : Page management
 */

#ifndef _PAGING_H_
#define _PAGING_H_

#include "os_type.h"

typedef enum PagePermissionFlag {
    READ = 0,
    READWRITE = 1,
} PagePermissionFlag;

typedef enum PagePriviledgeFlag {
    SUPERVISOR = 0,
    USER = 1,
} PagePriviledgeFlag;

class PageManager {
    private:
        static uint32 ID_MAPPED_TABLE_ADDR;     //The identity-mapped level 1 table for temporary mapping
    public:
        /**
         * Set the identity-mapped table address
         * 
         * @param addr the address of the identity-mapped level 1 table
         */
        static void setMappedTableAddr(uint32 addr);
        /**
         * Map a virtual page to a physical page, actually adds an entry into the page table
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         *
         * @param physicalAddr the address of physical page that is mapped to
         * @param virtualAddr the address of virtual page that is mapped from
         * @param permission the permission flag of the page
         * @param priviledge the priviledge flag of the page
         */
        static void mapPage(uint32 physicalAddr, uint32 virtualAddr, PagePermissionFlag permission, PagePriviledgeFlag priviledge);
        /**
         * Unmap a physical page from a virtual page, actually removes an entry from the page table
         * 
         * The function will NOT use ID_MAPPED_TABLE_ADDR if it's not initialized
         * 
         * @param physicalAddr the address of physical page that is to be unmapped
         * @param virtualAddr the address of virtual page that is to be mapped
         */
        static void unmapPage(uint32 physicalAddr, uint32 virtualAddr);
        /**
         * Temporarily map a physical page to a virtual address
         * 
         * @param physicalAddr the address of page to be mapped
         * @return the virtual address mapped
         */
        static uint32 allocPageMap(uint32 physicalAddr);
        /**
         * Free a temporarily mapped page
         * 
         * @param virtualAddr the address of page to be freed
         */
        static void freePageMap(uint32 virtualAddr);
};

#endif
