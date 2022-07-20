%include "boot.inc"
[org 0x7E00]
[bits 16]

; Print something
mov ax, 0xB800
mov gs, ax  ; Set video segment

mov si, 0
print:
    mov dl, byte [_msg + si]
    cmp dl, 0
    je  print_exit
    inc si
    ; Calculate cordinate in vga memory
    mov ax, word [_row]
    imul ax, 80
    add ax, word [_col]
    imul ax, 2
    ; Copy character to memory
    mov bx, ax
    mov byte [gs:bx], dl    ; Character to be printed
    mov byte [gs:bx + 1], 0x0F  ; Black background with white foreground
    ; Set new cordinate
    add word [_col], 1
    mov ax, word [_col]
    cmp ax, 80
    jne add_row_exit
    add word [_row], 1
    mov word [_col], 0
    add_row_exit:
    ; Reture to top
    jmp print
print_exit:

; Halt here
jmp $

; Variables
_row    dw 0
_col    dw 0
_msg    db 'Hello World!',
        db 0

times 2048 - ($ - $$) db 1