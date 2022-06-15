/*** 
 * Author       : Linloir
 * Date         : 2022-06-03 22:54:21
 * LastEditTime : 2022-06-15 09:42:36
 * Description  : 
 */

#include "os_constant.h"
#include "handler.h"
#include "interrupt.h"
#include "std_utils.h"
#include "framemanager.h"
#include "processmanager.h"
#include "paging.h"
#include "syscallmanager.h"

// extern "C" void emptyHandler() {
//     asm_disable_interrupt();
//     printf("Unhandled interrupt\n");
//     while(true){
//         //Halt
//     }
// }

Interrupt void emptyHandler(InterruptFrame* frame) {
    printf("Empty handler\n");
    while(true) {

    }
}

Interrupt void timeInterruptHandler(InterruptFrame* frame) {
    eoi();
    ProcessManager::onTimeTick();
    // Scheduler::onTimeInterrupt();
}

Interrupt void doubleFaultInterruptHandler(InterruptFrame* frame, uint32 errCode) {

    printf("Double fault, err code: %d\n", errCode);
    while(true) {
        
    }
}

Naked void syscallInterruptHandler() {
    //WARNING:
    //%%eax is used for return value
    //Therefore DO NOT restore state of eax
    
    //Preserve used registers
    asm volatile(
        "push %ebp\n\t"
        "movl %esp, %ebp\n\t"
        "push %gs\n\t"
        "push %fs\n\t"
        "push %es\n\t"
        "push %ds\n\t"
    );
    //Push parameters
    asm volatile(
        "push %eax\n\t"    //p4
        "push %ebx\n\t"    //p3
        "push %ecx\n\t"    //p2
        "push %edx\n\t"    //p1
        "push %esi\n\t"    //p0
        "push %edi\n\t"    //vec
    );
    //Push segment selectors
    asm volatile(
        "movl %[ds], %%eax\n\t"
        "movl %%eax, %%ds\n\t"
        "movl %%eax, %%es\n\t"
        "movl %%eax, %%fs\n\t"
        "movl %%eax, %%gs\n\t"
        :
        : [ds]"r"(DATA_SELECTOR)
    );
    //Call
    asm volatile(
        "call %P0\n\t"
        :
        : "i"(SyscallManager::_handler)
    );
    //Free stack
    asm volatile(
        "addl $24, %esp\n\t"
    );
    //Restore state
    asm volatile(
        "pop %ds\n\t"
        "pop %es\n\t"
        "pop %fs\n\t"
        "pop %gs\n\t"
        "pop %ebp\n\t"
    );
    //return
    asm volatile(
        "iret\n\t"
    );
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