/*** 
 * Author       : Linloir
 * Date         : 2022-07-24 22:15:50
 * LastEditTime : 2022-08-04 19:09:33
 * Description  : 
 */

#include "types.h"
#include "pgentry.h"

#ifndef _PGTABLE_H_
#define _PGTABLE_H_

class PageTable {
    private:
        PageTableEntry _entries[1024];
    public:
        static PageTable* from(const PageTableEntry& entry);
        static PageTable* from(const uint32 addr);
        PageTableEntry& at(uint32 idx);
        void init();
};

#endif