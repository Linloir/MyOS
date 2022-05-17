[bits 32]

global asm_atomic_exchange

asm_atomic_exchange:
    ; push &keyA
    ; push &keyB
    ; push ret
    push ebp
    mov ebp, esp
    push eax
    push ebx
    
    mov ebx, [ebp + 2 * 4]
    mov eax, [ebx]
    mov ebx, [ebp + 3 * 4]
    xchg eax, [ebx]

    pop eax
    pop ebx
    pop ebp

    ret
