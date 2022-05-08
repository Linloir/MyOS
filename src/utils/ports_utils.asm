global asm_ports_write
global asm_ports_read

; void asm_ports_write(uint16 port, uint8 value)
asm_ports_write:
    push ebp
    mov ebp, esp
    pushad
    
    mov ax, word [ebp + 2 * 4]  ; load port to ax
    mov dx, ax
    mov al, byte [ebp + 3 * 4]  ; load value to al
    out dx, al

    popad
    pop ebp
    ret

; uint8 asm_ports_read(uint16 port)
asm_ports_read:
    push ebp
    mov ebp, esp
    push edx

    xor eax, eax                ; erase eax
    mov dx, word[ebp + 2 * 4]   ; load port to dx
    in  al, dx                  ; read port

    pop edx
    pop ebp
    ret
