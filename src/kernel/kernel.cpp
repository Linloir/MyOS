/*** 
 * Author       : Linloir
 * Date         : 2022-05-15 22:14:20
 * LastEditTime : 2022-05-22 16:29:45
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
    // _non_lock_cheeseBurger_problem();    //assignment1
    // _spin_lock_cheeseBurger_problem();   //assignment1
    // _sem_lock_cheeseBurger_problem();    //assignment1

    // _non_lock_cp_problem();              //assignment2
    // cp_problem();                        //assignment2

    // _non_lock_phil_problem();            //assignment3
    // _naive_lock_phil_problem();          //assignment3
    // _dead_lock_phil_problem();           //assignment3
    // _tri_lock_phil_problem();            //assignment3
    // _bi_lock_phil_problem();             //assignment3
    
    while(true){
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
    while(true) {
        //Halt
    }
}