[bits 32]

global asm_load_gdtr

asm_load_gdtr:
    push ebp
    mov ebp, esp
    pushad

    mov eax, [ebp + 2 * 4]
    lgdt [eax]

    popad
    pop ebp
    ret