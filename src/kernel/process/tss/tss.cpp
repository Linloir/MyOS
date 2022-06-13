/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 14:51:41
 * LastEditTime : 2022-06-13 16:07:34
 * Description  : 
 */

#include "os_constant.h"
#include "tss.h"
#include "gdt.h"

TaskStateSegment TSS;

void TaskStateSegment::initialize() {
    //init TSS
    TSS = TaskStateSegment();
    TSS._esp0 = ESP_0_ADDR;
    TSS._ss0 = STACK_SELECTOR;
    //load tr
    TSS.loadTR();
}

void TaskStateSegment::loadTR() {
    short index = -1;
    for(int i = 0; i < GDT_MAX_SIZE; i++) {
        if(GDT[i].base() != (uint32)this) {
            continue;
        }
        index = i;
    }
    if(index == -1) {
        return;
    }
    index <<= 3;
    __asm__ (
        "ltr %[tr]\n\t"
        :
        : [tr]"r"(index)
    );
}