/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-06-01 10:11:50
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "proc.h"
#include "std_utils.h"
#include "lock.h"
#include "frame.h"
#include "paging.h"

void firstThread(void** args);

void firstThread(void**) {
    
    while(true){
    }
}

extern "C" void kernel() {
    clearScreen();
    printf("Kernel running at address %x\n", &kernel);
    uint32 memorySize = *(uint32*)0x7c00;
    printf("Total memory: %x(", memorySize);
    printf("%d KiB)\n", memorySize / 1024);
    
    //Init page table manager
    FrameManager::initialize(memorySize);
    printf("Total frames: %d\n", FrameManager::totalFrames());
    printf("Free frames: %d\n", FrameManager::availableFrames());

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
