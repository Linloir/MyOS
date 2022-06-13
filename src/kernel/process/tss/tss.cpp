/*** 
 * Author       : Linloir
 * Date         : 2022-06-13 14:51:41
 * LastEditTime : 2022-06-13 15:55:59
 * Description  : 
 */

#include "os_constant.h"
#include "tss.h"
#include "gdt.h"

TaskStateSegment TSS;

void TaskStateSegment::initialize() {
    //init TSS
    TSS._esp0 = ESP_0_ADDR;
    TSS._ss0 = STACK_SELECTOR;
    //load tr
    TSS.loadTR();
}

void TaskStateSegment::loadTR() {
    int index = -1;
    for(int i = 0; i < GDT_MAX_SIZE; i++) {
        if(GDT[i].base() != (uint32)this) {
            continue;
        }
        index = i;
    }
    if(index == -1) {
        return;
    }
    asm (
        "ltr %1;"
        :
        : "r"(index)
    );
}