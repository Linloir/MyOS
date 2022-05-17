/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 21:31:10
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "sched.h"
#include "std_utils.h"

int cheeseBurger = 0;

void firstThread() {
    while(true){
    }
}

void secondThread() {
    printf("There are %d cheese burgers\n", cheeseBurger);
    for(int i = 0; i < 200000000; i++){}
    printf("made 10 cheese burgers\n");
    printf("now there are %d cheese burgers\n", cheeseBurger);
    for(int i = 0; i < 200000000; i++){}
    if(cheeseBurger == 0) {
        printf("What the ? where are my cheese burgers!");
    }
    else {
        printf("Safe and sound");
    }
}

void thirdThread() {
    for(int i = 0; i < 200000000; i++){}
    printf("Look what I found! %d burgers!\n", cheeseBurger);
    for(int i = 0; i < 100000000; i++){}
    printf("Now there are %d cheese burgers\n", cheeseBurger);
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
    Scheduler::executeThread(secondThread, nullptr, 3);
    Scheduler::executeThread(thirdThread, nullptr, 3);
    while(true) {
        //Halt
    }
}