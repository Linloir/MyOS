/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 14:51:41
 * LastEditTime : 2022-06-14 20:05:46
 * Description  : 
 */

#include "os_constant.h"
#include "tss.h"
#include "gdt.h"
#include "mmu.h"

TaskStateSegment TSS;

void TaskStateSegment::initialize() {
    //init TSS
    TSS = TaskStateSegment();
    TSS._esp0 = ESP_0_ADDR;
    TSS._ss0 = STACK_SELECTOR;
    TSS._es = DATA_SELECTOR;
    TSS._cs = CODE_SELECTOR;
    TSS._ss = STACK_SELECTOR;
    TSS._ds = DATA_SELECTOR;
    TSS._fs = DATA_SELECTOR;
    TSS._gs = DATA_SELECTOR;
    //load tr
    TSS.loadTR();
}

void TaskStateSegment::loadTR() {
    asm(
        "ltr %[tr]\n\t"
        :
        : [tr]"r"((uint16)TSS_SELECTOR)
    );
}