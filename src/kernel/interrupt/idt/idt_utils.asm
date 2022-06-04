[bits 32]

global asm_load_idtr

asm_load_idtr:
    push ebp
    mov ebp, esp
    pushad

    mov eax, [ebp + 2 * 4]
    lidt [eax]

    popad
    pop ebp
    ret
