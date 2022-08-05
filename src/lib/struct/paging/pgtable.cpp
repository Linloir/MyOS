/*** 
 * Author       : Linloir
 * Date         : 2022-08-04 19:04:30
 * LastEditTime : 2022-08-04 19:51:33
 * Description  : Page Table
 */

#include "pgtable.h"

PageTable* PageTable::from(const PageTableEntry& entry) {
    return (PageTable*)entry.address();
}

PageTable* PageTable::from(const uint32 addr) {
    return (PageTable*)addr;
}

PageTableEntry& PageTable::at(uint32 idx) {
    return _entries[idx];
}

void PageTable::init() {
    for(int i = 0; i < 1024; i++) {
        _entries[i] = PageTableEntry();
    }
}