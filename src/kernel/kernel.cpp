#include "interrupt.h"
#include "os_constant.h"

extern "C" void kernel() {
    InterruptManager interruptManager = InterruptManager((uint32*)IDT_START_ADDR);
    interruptManager.set8259A();
    interruptManager.setTimeInterruptHandler(asm_time_interrupt_handler);
    interruptManager.enableTimeInterrupt();
    while(true) {
        //Halt
    }
}