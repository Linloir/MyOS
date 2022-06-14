[bits 32]

global asm_switch_process

asm_switch_process:
    ; new esp
    ; table
    ; *old esp
    ; ret
    push ebp
    mov ebp, esp

    push ds
    push es
    push fs
    push gs

    push esi
    push edi
    push ebp

    push edx
    push ecx
    push ebx
    push eax

    ; pushad

    mov dword eax, [ebp + 2 * 4]  ; eax = *oldEsp
    mov dword [eax], esp    ; *oldEsp = esp

    mov dword eax, [ebp + 3 * 4]  ; eax = table
    mov cr3, eax    ; cr3 = table

    mov dword eax, [ebp + 4 * 4]    ; eax = newEsp
    mov esp, eax

    ; popad

    pop eax
    pop ebx
    pop ecx
    pop edx

    pop ebp
    pop edi
    pop esi
    
    pop gs
    pop fs
    pop es
    pop ds

    pop ebp
    ret