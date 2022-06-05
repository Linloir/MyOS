[bits 32]

global asm_load_gdtr

asm_load_gdtr:
    push ebp
    mov ebp, esp
    pushad

    mov eax, [ebp + 2 * 4]
    lgdt [eax]

    mov eax, 0x08
    mov cs, eax
    mov eax, 0x10
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov eax, 0x18
    mov ss, eax

    popad
    pop ebp
    ret