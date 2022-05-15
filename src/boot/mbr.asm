%include "boot.inc"
org 0x7c00
[bits 16]

; Main Program-----------------------

; Init registers
xor ax, ax
mov ds, ax
; mov cs, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; Init stack pointer
mov sp, 0x7C00

; Load boot loader to memory
load_bootloader:
    mov ax, BL_SECTOR_START
    mov bx, BL_START_ADDR
    mov cx, BL_SECTOR_COUNT
    mov dx, 0x0
    .read_sector:
        push bx
        push dx
        push ax
        
        call asm_read_sector

        add sp, 6

        add ax, 1
        add bx, 512

        loop .read_sector

; Jump to boot loader
jump_bootloader:
    jmp 0x0000:BL_START_ADDR

; Dead loop
jmp $

; Functions & Variables--------------

; Read hard disk using LBA28 method
asm_read_sector:
    ; push readToAddr
    ; push sector[27:16]
    ; push sector[15:0]
    ; push ret

    push bp                     ; Save bp
    mov bp, sp

    pushad

    .prepare:
        mov ax, [bp + 2 * 2]    ; load sector [15:0] from stack

        mov dx, 0x1F3
        out dx, al           ; sector [7:0]

        inc dx
        mov al, ah
        out dx, al           ; sector [15:8]
        
        mov ax, [bp + 3 * 2]    ; load sector [27:16] from stack

        inc dx
        out dx, al           ; sector [23:16]

        and ax, 0x0FFF          ; set [31:28] to zero
        or  ax, 0xE000          ; set [31:28] to 0b1110
        inc dx
        mov al, ah
        out dx, al           ; sector [31:28]

        mov dx, 0x1F2
        mov al, 0x01
        out dx, al         ; read one sector

        mov al, 0x20             ; read command
        mov dx, 0x1F7
        out dx, al           ; send read command
    
    .wait:
        mov dx, 0x1F7
        in  al, dx            ; read status
        and al, 0x88            ; ignore bit 0 (err) and bits 1-2, bits 4-6
        cmp al, 0x08            ; judge wheter bit 7 is 0 and bit 3 is 1
        jnz .wait
    
    .write:
        mov bx, [bp + 4 * 2]    ; load address from stack
        mov cx, 256             ; load loop count
        mov si, 0               ; init offset
        .read_byte:
            mov dx, 0x1F0
            in  ax, dx       ; read 2 bytes
            mov [bx + si], ax       ; write to memory
            add si, 2           ; add 2 to offset
            loop .read_byte

    popad
    pop bp
    
    ret

times 510 - ($ - $$) db 0
db 0x55,0xAA