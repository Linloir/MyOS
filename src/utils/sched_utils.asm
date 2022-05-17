[bits32]

global asm_switch_thread

asm_switch_thread:
    ; push next
    ; push cur
    ; push ret

    push ebp
    push ebx
    push edi
    push esi

    mov ebp, esp

    mov eax, [ebp + 5 * 4]
    mov [eax], esp

    mov eax, [ebp + 6 * 4]
    mov esp, [eax]

    pop esi
    pop edi
    pop ebx
    pop ebp

    sti ; important
    ret