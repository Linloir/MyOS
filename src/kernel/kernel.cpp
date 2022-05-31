/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-31 15:35:09
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "proc.h"
#include "std_utils.h"
#include "lock.h"

#include "paging.h"

uint32 PageManager::ID_MAPPED_TABLE_ADDR = NULL;

void firstThread(void** args);

void firstThread(void**) {
    uint32 memorySize = *(uint32*)0x7c00;
    uint32* secondLevelPageTableAddr = (uint32*)0x100000;
    uint32* firstLevelPageTableAddr = (uint32*)0x101000;
    //Map the first 1MiB of memory
    uint32 physAddr = 0x0;
    uint32 pageFlags = 0x7;
    for(int i = 0; i < 256; i++) {
        uint32 pageEntry = physAddr | pageFlags;
        firstLevelPageTableAddr[i] = pageEntry;
        physAddr += PAGE_SIZE;
    }
    secondLevelPageTableAddr[0] = (uint32)firstLevelPageTableAddr | pageFlags;
    printf("%x", memorySize);
    
    while(true){
    }
}

extern "C" void kernel() {
    clearScreen();
    InterruptManager::initialize((uint32*)IDT_START_ADDR);
    Scheduler::initialize();
    InterruptManager::set8259A();
    InterruptManager::setTimeInterruptHandler(asm_time_interrupt_handler);
    InterruptManager::enableTimeInterrupt();
    asm_enable_interrupt();
    Scheduler::executeThread(firstThread, nullptr, 1);
    while(true) {
        //Halt
    }
}