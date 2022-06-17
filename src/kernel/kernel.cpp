/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-06-17 13:31:44
 * Description  : 
 */
#include "interrupt.h"
#include "systemio.h"
#include "display.h"
#include "kernel.h"
#include "lock.h"
#include "idt.h"
#include "framemanager.h"
#include "processmanager.h"
#include "paging.h"
#include "mmu.h"
#include "allocator.h"
#include "gdt.h"
#include "tss.h"
#include "syscallmanager.h"
#include "systemcall.h"

#include "lab.h"

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
    printf("    Mapped address space: \n        [virtual] 0x%x ~ 0x%x -> [physical] 0x%x ~ 0x%x\n", 0xC0100000, 0xC0100000 + memorySize, 0x0, memorySize);
    printf("    Total memory: %d KiB (%d MiB)\n", totalFrames << 2, totalFrames >> 8);
    printf("}\n");
    
    initHeap(heapAddr);
    int* test = (int*)malloc(sizeof(int));
    *test = 1;
    if(*test == 1)
        printf("Heap initialized!\n");
    else
        printf("Heap initializing err: 0x%x -> %d\n", test, *test);
    
    FrameManager::initialize(totalFrames, 256 + 2 + mappedPages + heapFrameCount);
    PageManager::initialize();
    GDTManager::initialize();
    TSSManager::initialize();
    ProcessManager::initialize();
    SyscallManager::initialize();
    initInterrupt();

    lab8();
    
    while(true) {
        // printf("Kernel\n");
        // for(int i = 0; i < 0xFFFFFFF; i++) {}
    }
}

void initHeap(uint32 heapStartAddress) {
    malloc_init((void*)(heapStartAddress));
}

void initInterrupt() {
    uint32 idtAddr = FrameManager::allocateFrame(FrameFlag::LOCKED)->physicalAddr();
    InterruptDescriptorTable* idt = InterruptDescriptorTable::fromPhysicalAddr(idtAddr);
    idt->initialize();
    idt->loadToIDTR();
    set8259A();
    disableTimeInterrupt();
    enableTimeInterrupt();
    setInterruptStatus(true);
}