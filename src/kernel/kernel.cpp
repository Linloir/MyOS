/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-18 15:54:54
 * Description  : 
 */
#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"
#include "proc.h"
#include "std_utils.h"
#include "lock.h"

#include "cb_problem.h"
#include "cp_problem.h"
#include "phil_problem.h"

void firstThread(void** args);

void firstThread(void**) {
    // _non_lock_cheeseBurger_problem();
    // _spin_lock_cheeseBurger_problem();
    // _sem_lock_cheeseBurger_problem();

    // _non_lock_cp_problem();
    // cp_problem();

    // _non_lock_phil_problem();
    // _dead_lock_phil_problem();
    // _tri_lock_phil_problem();
    _bi_lock_phil_problem();
    
    while(true){
    }
}

void philosopher(void** args) {
    
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