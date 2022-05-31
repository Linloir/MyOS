global asm_set_cr3

asm_set_cr3:
    push ebp
    mov ebp, esp

    push eax

    mov eax, [ebp + 2 * 4]
    mov cr3, eax
    
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    pop eax
    pop ebp

    ret