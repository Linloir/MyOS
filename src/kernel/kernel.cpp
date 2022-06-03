/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-06-03 15:40:59
 * Description  : 
 */
#include "interrupt.h"
#include "kernel.h"
#include "proc.h"
#include "std_utils.h"
#include "lock.h"
#include "frame.h"
#include "paging.h"
#include "mmu.h"
#include "allocator.h"

void firstThread(void** args);

void firstThread(void**) {
    
    while(true){
    }
}

extern "C" void kernel() {
    clearScreen();

    int* info = (int*)physicToVirtual(MEMORY_INFO_ADDR);
    int totalFrames = info[1];
    int mappedPages = info[2];
    uint32 memorySize = (uint32)totalFrames << 12;

    printf("Kernel: {\n");
    printf("    'void kernel()' address: 0x%x\n", &kernel);
    printf("    Mapped pages: %d\n", mappedPages);
    printf("    Mapped address space: \n        [virtual] 0x%x ~ 0x%x -> [physical] 0x%x ~ 0x%x\n", 0xB0100000, 0xB0100000 + memorySize, 0x0, memorySize);
    printf("    Total memory: %d KiB (%d MiB)\n", totalFrames << 2, totalFrames >> 8);
    printf("}\n");

    initHeap();
    int* test = (int*)malloc(sizeof(int));
    *test = 1;
    if(*test == 1)
        printf("Heap initialized!\n");
    else
        printf("Heap initializing err: 0x%x -> %d\n", test, *test);
    
    //Init page table manager
    FrameManager::initialize();
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

void initHeap() {
    uint32* info = (uint32*)physicToVirtual(MEMORY_INFO_ADDR);
    uint32 heapStartAddress = info[4];
    printf("heap start at: 0x%x\n", heapStartAddress);
    malloc_init((void*)(heapStartAddress));
}
