[bits 32]

global asm_empty_interrupt_handler
global asm_time_interrupt_handler
global asm_double_fault_interrupt_handler
global asm_page_fault_interrupt_handler

extern emptyHandler
extern timeInterruptHandler
extern doubleFaultInterruptHandler
extern pageFaultInterruptHandler

; asm_empty_interrupt_handler:
;     pushad

;     mov al, 0x20
;     out 0x20, al
;     out 0xA0, al

;     call emptyHandler

;     popad
;     iret

asm_time_interrupt_handler:
    pushad

    mov al, 0x20
    out 0x20, al
    out 0xA0, al

    call timeInterruptHandler

    popad
    iret

asm_double_fault_interrupt_handler:
    pushad

    mov al, 0x20
    out 0x20, al
    out 0xA0, al

    call doubleFaultInterruptHandler

    popad
    iret

; asm_page_fault_interrupt_handler:
;     push ebp
;     mov ebp, esp
;     pushad

;     mov al, 0x20
;     out 0x20, al
;     out 0xA0, al

;     mov eax, cr2
;     push eax

;     mov eax, [ebp + 1 * 4]  ; err code
;     push eax
;     call pageFaultInterruptHandler
;     add esp, 8

;     popad
;     pop ebp
;     add esp, 4
;     iret