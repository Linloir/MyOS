/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-04 18:42:15
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
    // while(true){}
    // Scheduler::onTimeInterrupt();
    printf("test\n");
}

extern "C" void doubleFaultInterruptHandler() {
    printf("Double fault\n");
    while(true) {
        
    }
}

extern "C" void pageFaultInterruptHandler(uint32 virtualAddr) {
    printf("Page fault when visiting 0x%x\n", virtualAddr);
    uint32 physAddr = FrameManager::physicalFrames.allocateFrame();
    PageManager::mapPage(virtualAddr, physAddr, PageFlag::PRESENT | PageFlag::WRITABLE);
}