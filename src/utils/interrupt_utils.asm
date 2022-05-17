[bits 32]

extern emptyHandler
extern timeInterruptHandler

global asm_load_idtr
global asm_interrupt_status
global asm_enable_interrupt
global asm_disable_interrupt
global asm_empty_interrupt_handler
global asm_time_interrupt_handler

asm_load_idtr:
    push ebp
    mov ebp, esp
    pushad

    mov ebx, idtr

    mov eax, [ebp + 2 * 4]
    mov [ebx + 2], eax

    mov ax, [ebp + 3 * 4]
    mov [ebx], ax

    lidt [idtr]

    popad
    pop ebp
    ret

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

idtr dw 0
     dd 0