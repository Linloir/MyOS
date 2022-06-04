[bits 32]

global asm_interrupt_status
global asm_enable_interrupt
global asm_disable_interrupt

asm_interrupt_status:
    xor eax, eax
    pushfd
    pop eax
    and eax, 0x200
    ret

asm_enable_interrupt:
    sti
    ret

asm_disable_interrupt:
    cli
    ret