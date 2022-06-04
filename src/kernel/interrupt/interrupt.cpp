/*** 
 * Author       : Linloir
 * Date         : 2022-06-04 11:35:31
 * LastEditTime : 2022-06-04 14:59:26
 * Description  : 
 */

#include "interrupt.h"
#include "ports_utils.h"

void set8259A() {
    uint8 ICW1M = 0x11;
    uint8 ICW2M = IRQ0_8259A_MASTER;
    uint8 ICW3M = 0x04;
    uint8 ICW4M = 0x01;
    uint8 ICW1S = 0x11;
    uint8 ICW2S = IRQ0_8259A_SLAVE;
    uint8 ICW3S = 0x02;
    uint8 ICW4S = 0x01;
    asm_ports_write(0x20, ICW1M);
    asm_ports_write(0xA0, ICW1S);
    asm_ports_write(0x21, ICW2M);
    asm_ports_write(0xA1, ICW2S);
    asm_ports_write(0x21, ICW3M);
    asm_ports_write(0xA1, ICW3S);
    asm_ports_write(0x21, ICW4M);
    asm_ports_write(0xA1, ICW4S);
}

void enableTimeInterrupt() {
    uint8 OCW = asm_ports_read(0x21);
    OCW &= 0xFE;
    asm_ports_write(0x21, OCW);
}

void disableTimeInterrupt() {
    uint8 OCW = asm_ports_read(0x21);
    OCW |= 0x01;
    asm_ports_write(0x21, OCW);
}

bool getInterruptStatus() {
    return asm_interrupt_status() ? true: false;
}

void setInterruptStatus(bool status) {
    if(status) {
        asm_enable_interrupt();
    }
    else {
        asm_disable_interrupt();
    }
}