/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-06-08 11:43:25
 * Description  : 
 */
#include "interrupt.h"
#include "std_utils.h"
#include "display_utils.h"
#include "kernel.h"
#include "proc.h"
#include "lock.h"
#include "idt.h"
#include "framemanager.h"
#include "paging.h"
#include "mmu.h"
#include "allocator.h"
#include "gdt.h"

void firstThread(void** args);

void firstThread(void**) {

    int* test = (int*)0xBF000000;
    *test = 1;
    printf("%d\n", *test);
    
    while(true){
    }
}

extern "C" void kernel() {
    clearScreen();

    int* info = (int*)toVirtualAddress(MEMORY_INFO_ADDR);
    int totalMemorySize = info[0];
    int totalFrames = info[1];
    int mappedPages = info[2];
    uint32 scndTableAddr = (uint32)info[3];
    uint32 heapAddr = (uint32)info[4];
    int heapFrameCount = info[5];

    uint32 memorySize = (uint32)totalFrames << 12;

    printf("Kernel: {\n");
    printf("    'void kernel()' address: 0x%x\n", &kernel);
    printf("    Mapped pages: %d\n", mappedPages);
    printf("    Mapped address space: \n        [virtual] 0x%x ~ 0x%x -> [physical] 0x%x ~ 0x%x\n", 0xB0100000, 0xB0100000 + memorySize, 0x0, memorySize);
    printf("    Total memory: %d KiB (%d MiB)\n", totalFrames << 2, totalFrames >> 8);
    printf("}\n");
    
    initHeap(heapAddr);
    int* test = (int*)malloc(sizeof(int));
    *test = 1;
    if(*test == 1)
        printf("Heap initialized!\n");
    else
        printf("Heap initializing err: 0x%x -> %d\n", test, *test);
    
    initFrames(totalFrames, 256 + 2 + mappedPages + heapFrameCount);
    initPaging();
    initGlobalDescriptorTable();
    initScheduler();
    initInterrupt();

    Scheduler::executeThread(firstThread, 0, 1);
    while(true) {
        //Halt
    }
}

void initHeap(uint32 heapStartAddress) {
    malloc_init((void*)(heapStartAddress));
}

void initFrames(int totalFrames, int mappedFrames) {
    FrameManager::initialize(totalFrames, mappedFrames);
}

void initPaging() {
    PageManager::initialize();
}

void initGlobalDescriptorTable() {
    GDT.initialize();
    GDT.loadToGDTR();
}

void initScheduler() {
    Scheduler::initialize();
}

void initInterrupt() {
    uint32 idtAddr = FrameManager::allocateFrame().physicalAddr();
    InterruptDescriptorTable* idt = InterruptDescriptorTable::fromPhysicalAddr(idtAddr);
    idt->initialize();
    idt->loadToIDTR();
    set8259A();
    disableTimeInterrupt();
    enableTimeInterrupt();
    setInterruptStatus(true);
}