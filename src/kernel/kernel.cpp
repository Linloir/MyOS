#include "interrupt.h"
#include "os_constant.h"
#include "kernel.h"

extern "C" void kernel() {
    clearScreen();
    InterruptManager interruptManager = InterruptManager((uint32*)IDT_START_ADDR);
    interruptManager.set8259A();
    interruptManager.setTimeInterruptHandler(asm_time_interrupt_handler);
    interruptManager.enableTimeInterrupt();
    asm_enable_interrupt();
    while(true) {
        //Halt
    }
}