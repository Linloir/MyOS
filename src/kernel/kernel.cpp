/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-17 16:48:27
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "sched.h"
#include "display_utils.h"

void firstThread() {
    setCursor(0);
    print("hello, world");
    while(true){
        print("hello, world", 0, 0);
    }
}

void secondThread() {
    for(int i = 0; i < 200000; i++){
        print("thread 2              ", 0, 0);
    }
}

void thirdThread() {
    for(int i = 0; i < 200000; i++){
        print("thread 3              ", 0, 0);
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