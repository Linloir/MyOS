[bits 32]

global asm_empty_interrupt_handler
global asm_time_interrupt_handler
global asm_page_fault_interrupt_handler

extern emptyHandler
extern timeInterruptHandler
extern pageFaultInterruptHandler

asm_empty_interrupt_handler:
    pushad

    call emptyHandler

    popad

    mov al, 0x20
    out 0x20, al
    out 0xA0, al
    iret

asm_time_interrupt_handler:
    pushad

    mov al, 0x20
    out 0x20, al
    out 0xA0, al

    call timeInterruptHandler

    popad
    iret

asm_page_fault_interrupt_handler:
    pushad

    mov al, 0x20
    out 0x20, al
    out 0xA0, al

    mov eax, cr2
    push eax
    call pageFaultInterruptHandler
    pop eax

    popad
    iret