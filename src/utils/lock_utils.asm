[bits 32]

global asm_atomic_exchange
global asm_atomic_test

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
    mov ebx, [ebp + 2 * 4]
    mov [ebx], eax

    pop eax
    pop ebx
    pop ebp

    ret

asm_atomic_test:
    ; push &key
    ; push ret
    push ebp
    mov ebp, esp

    mov ebx, [ebp + 2 * 4]
    lock bts dword [eax], 0
    pushf
    pop eax
    and eax, 1
    
    pop ebp

    ret