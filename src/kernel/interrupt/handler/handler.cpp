/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-05 11:22:12
 * Description  : 
 */

#include "handler.h"
#include "interrupt.h"
#include "proc.h"
#include "std_utils.h"
#include "frame.h"
#include "paging.h"

extern "C" void emptyHandler() {
    asm_disable_interrupt();
    printf("Unhandled interrupt\n");
    while(true){
        //Halt
    }
}

extern "C" void timeInterruptHandler() {
    Scheduler::onTimeInterrupt();
}

extern "C" void doubleFaultInterruptHandler() {
    printf("Double fault\n");
    while(true) {
        
    }
}

extern "C" void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr) {
    printf("Page fault with err code 0x%x when visiting 0x%x\n", errCode, virtualAddr);
    uint32 physAddr = FrameManager::physicalFrames.allocateFrame();
    PageManager::mapPage(virtualAddr, physAddr, PageFlag::PRESENT | PageFlag::WRITABLE);
}