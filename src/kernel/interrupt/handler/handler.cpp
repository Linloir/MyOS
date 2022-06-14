/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-14 14:10:28
 * Description  : 
 */

#include "handler.h"
#include "interrupt.h"
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
    printf("Empty handler\n");
    while(true) {

    }
}

__attribute__ ((interrupt)) void timeInterruptHandler(InterruptFrame* frame) {
    eoi();
    ProcessManager::onTimeTick();
    // Scheduler::onTimeInterrupt();
}

__attribute__ ((interrupt)) void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode) {

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

void eoi() {
    asm(
        ".intel_syntax noprefix\n\t"
        "mov al, 0x20\n\t"
        "out 0x20, al\n\t"
        "out 0xA0, al\n\t"
    );
}