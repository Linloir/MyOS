/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-06-17 09:47:20
 * Description  : 
 */
#include "interrupt.h"
#include "std_utils.h"
#include "display_utils.h"
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

void firstThread();

void firstThread() {

    // int* test = (int*)0xBF000000;
    // *test = 1;
    // printf("Hello World\n");
    
    // uint32 result = sysTest(0, 1, 2, 3, 4);
    // sysTest(result, 0, 0, 0, 0);
    printNum(1, 2, 3, 4, 5);
    uint32 pid = fork();
    printNum(pid, 0, 0, 0, 0);
    if(pid == 0) {
        pid = fork();
        printNum(pid, 0, 0, 0, 0);
    }

    while(true) {
        // printf("Process 1\n");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
        // asm("hlt");
    }
}

void scndThread() {

    // int* test = (int*)0xBF000000;
    // *test = 1;
    printf("Hello World\n");
    
    while(true) {
        printf("Process 2\n");
        asm("hlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\t");
        asm("hlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\thlt\n\t");
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

    Process* frstProcess = (Process*)malloc(sizeof(Process));
    *frstProcess = Process(
        ProcessPriviledge::KERNEL,
        ProcessSegment(0x0, 0x0),
        ProcessManager::processOfPID(0),
        30,
        (uint32)firstThread
    );

    // Process* scndProcess = (Process*)malloc(sizeof(Process));
    // *scndProcess = Process(
    //     ProcessManager::allocPID(),
    //     ProcessPriviledge::USER,
    //     ProcessSegment::defaultUserDataSegment(),
    //     ProcessSegment::defaultUserStackSegment(),
    //     ProcessManager::processOfPID(0),
    //     30,
    //     (uint32)scndThread
    // );

    ProcessManager::execute(frstProcess);
    // ProcessManager::executeProcess(scndProcess);
    
    while(true) {
        printf("Kernel Process\n");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
        asm("hlt");
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