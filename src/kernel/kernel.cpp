/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 22:07:06
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "sched.h"
#include "std_utils.h"
#include "lock.h"

void firstThread(void** args);
void secondThread(void** args);
void thirdThread(void** args);

void firstThread(void**) {
    int cheeseBurger = 0;
    SpinLock spinLock = SpinLock();
    SemLock semLock = SemLock(1);
    void* args[3] = {&cheeseBurger, &spinLock, &semLock};
    Scheduler::executeThread(secondThread, args, 3);
    Scheduler::executeThread(thirdThread, args, 3);
    while(true){
    }
}

void secondThread(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* spinLock = (SpinLock*)args[1];
    SemLock* semLock = (SemLock*)args[2];
    // spinLock.lock();
    semLock->acquire();
    printf("There are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    (*cheeseBurger) += 10;
    printf("made 10 cheese burgers\n");
    printf("now there are %d cheese burgers\n", (*cheeseBurger));
    for(int i = 0; i < 200000000; i++){}
    if((*cheeseBurger) == 0) {
        printf("What the ? where are my cheese burgers!");
    }
    else {
        printf("Safe and sound\n");
    }
    semLock->release();
    // spinLock.release();
}

void thirdThread(void** args) {
    int* cheeseBurger = (int*)args[0];
    SpinLock* spinLock = (SpinLock*)args[1];
    SemLock* semLock = (SemLock*)args[2];
    // spinLock.lock();
    semLock->acquire();
    for(int i = 0; i < 200000000; i++){}
    printf("Look what I found! %d burgers!\n", (*cheeseBurger));
    for(int i = 0; i < 100000000; i++){}
    if((*cheeseBurger) >= 10) {
        (*cheeseBurger) -= 10;
    }
    printf("Now there are %d cheese burgers\n", (*cheeseBurger));
    semLock->release();
    // spinLock.lock();
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