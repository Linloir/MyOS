[org 0x7C00]  ; Indicates codes below starts at 0x7C00
[bits 16]   ; Indicates codes below run in 16-bit mode

; Initialize registers
xor ax, ax  ; ax = 0
mov ds, ax  ; ds = 0
mov ss, ax  ; ss = 0
mov es, ax  ; es = 0
mov fs, ax  ; fs = 0
mov gs, ax  ; gs = 0

; Initialize stack pointer
mov sp, 0x7C00

; Print something
mov ax, 0xB800
mov gs, ax  ; Set video segment as 0xB800

mov si, 0   ; Current character index
print:
    mov dl, byte [_msg + si]    ; Fetch character
    cmp dl, 0                   ; Exit if character is '\0'
    je  print_exit
    inc si                      ; index += 1
    ; Calculate cordinate in vga memory
    ; Shift = 2 * (80 * row  + col)
    mov ax, word [_row] 
    imul ax, 80
    add ax, word [_col]
    imul ax, 2
    ; Copy character to memory
    mov bx, ax
    mov byte [gs:bx], dl        ; Character to be printed
    mov byte [gs:bx + 1], 0x0F  ; Black background with white foreground
    ; Set new cordinate
    add word [_col], 1
    mov ax, word [_col]
    cmp ax, 80
    jne add_row_exit
    add word [_row], 1          ; move cursor to next row if col == 80
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

; Fill 0 before byte 447
times 446 - ($ - $$) db 0

; Fill 0 (temporary) for partition table
times 510 - ($ - $$) db 0

; Magic number
dw 0xAA55