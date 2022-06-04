[bits 32]

global getCR3
global setCR3

getCR3:
    push ebp
    mov eax, cr3
    pop ebp
    ret

setCR3:
    push ebp
    mov ebp, esp
    pushad

    mov eax, [ebp + 2 * 4]
    mov cr3, eax

    popad
    pop ebp
    ret