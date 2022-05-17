/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 21:15:21
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "sched.h"
#include "std_utils.h"

void firstThread() {
    while(true){
        printf("hello, world\n");
        for(int i = 0; i < 200000000; i++){}
    }
}

void secondThread() {
    for(int i = 0; i < 50; i++){
        printf("thread 2\n");
        for(int i = 0; i < 200000000; i++){}
    }
}

void thirdThread() {
    for(int i = 0; i < 50; i++){
        printf("thread 3\n");
        for(int i = 0; i < 200000000; i++){}
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
    Scheduler::executeThread(secondThread, nullptr, 1);
    Scheduler::executeThread(thirdThread, nullptr, 1);
    while(true) {
        //Halt
    }
}