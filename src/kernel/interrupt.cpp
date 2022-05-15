#include "interrupt.h"

void InterruptManager::set8259A() {
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

InterruptManager::InterruptManager(uint32* tableAddr) {
    descriptorTable = tableAddr;

    //Load default descriptor
    // InterruptDescriptor defaultDescriptor = InterruptDescriptor(CODE_SELECTOR, (uint32*)InterruptHandlers::emptyHandler, 1, 0, 1);
    InterruptDescriptor defaultDescriptor = InterruptDescriptor(CODE_SELECTOR, asm_empty_interrupt_handler, 1, 0, 1);
    for(int i = 0; i < IDT_MAX_SIZE; i++) {
        setDescriptor(defaultDescriptor, i);
    }

    //Load idtr
    asm_load_idtr(descriptorTable, IDT_MAX_SIZE * 8 - 1);
}

void InterruptManager::setDescriptor(InterruptDescriptor newDescriptor, uint32 id) {
    //Check id
    if(id >= IDT_MAX_SIZE) {
        return;
    }
    //Decode descriptor
    uint32 lo = 0x0;
    uint32 hi = 0x0;
    lo |= newDescriptor.handlerOffset & 0xFFFF;
    lo |= ((uint32)newDescriptor.handlerSegmentSelector << 16) & 0xFFFF0000;
    hi |= newDescriptor.handlerOffset & 0xFFFF0000;
    hi |= ((uint32)newDescriptor.pBit << 15) & 0x8000;
    hi |= ((uint32)newDescriptor.dplBits << 13) & 0x6000;
    hi |= ((((uint32)newDescriptor.dBit << 3) | 0x6) << 8) & 0x0F00;
    //Set descriptor
    descriptorTable[id * 2] = lo;
    descriptorTable[id * 2 + 1] = hi;
}

void InterruptManager::enableTimeInterrupt() {
    uint8 OCW = asm_ports_read(0x21);
    OCW &= 0xFE;
    asm_ports_write(0x21, OCW);
}

void InterruptManager::disableTimeInterrupt() {
    uint8 OCW = asm_ports_read(0x21);
    OCW |= 0x01;
    asm_ports_write(0x21, OCW);
}

void InterruptManager::setTimeInterruptHandler(void (*handler)()) {
    InterruptDescriptor timeInterruptDescriptor = InterruptDescriptor(CODE_SELECTOR, handler, 1, 0, 1);
    setDescriptor(timeInterruptDescriptor, IRQ0_8259A_MASTER);
}

bool InterruptManager::getInterruptStatus() {
    return asm_interrupt_status() ? true: false;
}

void InterruptManager::setInterruptStatus(bool status) {
    if(status) {
        asm_enable_interrupt();
    }
    else {
        asm_disable_interrupt();
    }
}

InterruptDescriptor::InterruptDescriptor(uint16 selector, void (*offset)(), uint8 p, uint8 dpl, uint8 d) {
    handlerSegmentSelector = selector;
    handlerOffset = (uint32)offset;
    pBit = p;
    dplBits = dpl;
    dBit = d;
}

extern "C" void emptyHandler() {
    asm_disable_interrupt();
    print("Unhandled interrupt");
    while(true){
        //Halt
    }
}

char time = '0';

extern "C" void timeInterruptHandler() {
    print(time);
    time += 1;
    if(time > '9') {
        time = '0';
    }
}