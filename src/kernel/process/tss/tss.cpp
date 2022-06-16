/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 14:51:41
 * LastEditTime : 2022-06-16 20:30:50
 * Description  : 
 */

#include "os_constant.h"
#include "gdt.h"
#include "tss.h"
#include "gdt.h"
#include "mmu.h"

TaskStateSegment TSSManager::_tss;

void TSSManager::_load() {
    asm(
        "ltr %[tr]\n\t"
        :
        : [tr]"r"((uint16)TSS_SELECTOR)
    );
}

void TSSManager::initialize() {
    //init TSS
    _tss = TaskStateSegment();
    _tss._esp0 = ESP0_STACK_TOP;
    _tss._ss0 = STACK_SELECTOR;
    _tss._es = DATA_SELECTOR;
    _tss._cs = CODE_SELECTOR;
    _tss._ss = STACK_SELECTOR;
    _tss._ds = DATA_SELECTOR;
    _tss._fs = DATA_SELECTOR;
    _tss._gs = DATA_SELECTOR;
    //insert into gdt
    GlobalDescriptor tssDescriptor = GlobalDescriptor(
        (uint32)&_tss,
        sizeof(_tss) - 1,
        0,
        GlobalDescriptorFlag::PRESENT | GlobalDescriptorFlag::IS_TSS | 
        GlobalDescriptorFlag::IS_32_BIT | GlobalDescriptorFlag::USE_32_BIT
    );
    GDTManager::set(TSS_SELECTOR >> 3, tssDescriptor);
    //load tr
    _load();
}