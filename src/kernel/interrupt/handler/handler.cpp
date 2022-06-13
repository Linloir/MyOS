/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-13 10:15:10
 * Description  : 
 */

#include "handler.h"
#include "interrupt.h"
#include "proc.h"
#include "std_utils.h"
#include "framemanager.h"
#include "processmanager.h"
#include "paging.h"

// extern "C" void emptyHandler() {
//     asm_disable_interrupt();
//     printf("Unhandled interrupt\n");
//     while(true){
//         //Halt
//     }
// }

__attribute__ ((interrupt)) void emptyHandler(InterruptFrame* frame) {
    register uint32 out asm("%al");
    printf("0x%x\n", frame);
    printf("Empty handler\n");
    while(true) {

    }
}

extern "C" void timeInterruptHandler() {
    printf("Time handler\n");
    // Scheduler::onTimeInterrupt();
}

extern "C" void doubleFaultInterruptHandler() {

    printf("Double fault\n");
    while(true) {
        
    }
}

// extern "C" void pageFaultInterruptHandler(uint32 errCode, uint32 virtualAddr) {
//     printf("Page fault with err code 0x%x when visiting 0x%x\n", errCode, virtualAddr);
//     Process* currentProcess = ProcessManager::curProcess();
//     PageTable* table = currentProcess->pageTable();

//     if(currentProcess->isData(virtualAddr)) {
//         PageTable* frstTable = PageTable::fromPhysicalAddr(table->entryAt(SCND_LEVEL_INDEX_MASK(virtualAddr)).address());
//         PageTableEntry* frstEntry = &frstTable->entryAt(FRST_LEVEL_INDEX_MASK(virtualAddr));
//         Frame newFrame = FrameManager::allocateFrame();
//         SwapManager::swapIn((SwapRecord*)frstEntry, newFrame);
//         return;
//     }

//     if(currentProcess->isStack(virtualAddr)) {
//         Page p = Page(virtualAddr);
//         while(!PageManager::isMapped(table, p)) {
//             Frame newFrame = FrameManager::allocateFrame();
//             PageFlag flags = PageFlag::PRESENT | PageFlag::WRITABLE;
//             if(currentProcess->priviledge() != ProcessPriviledge::KERNEL) {
//                 flags = flags | PageFlag::USER_ACCESSIBLE;
//             }
//             PageManager::mapPage(table, p, newFrame, flags);
//             p = p.higher();
//         }
//         return;
//     }

//     printf("Visiting wrong address\n");
// }