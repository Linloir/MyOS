%include "boot.inc"
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

; Read bootloader
push BOOTLOADER_LOAD_ADDRESS
push BOOTLOADER_SECTOR_COUNT
push BOOTLOADER_SECTOR_START_27_16
push BOOTLOADER_SECTOR_START_15_0
call read_sectors
add sp, 8

; Jump to bootloader
jmp BOOTLOADER_LOAD_ADDRESS

; Halt here
jmp $

; Loader sector function
; Notice: Loads using LBA method
read_sectors:
    ; push targetAddr
    ; push sectorCount
    ; push startSector[27:16]
    ; push startSector[15:0]
    ; push ret
    push sp
    mov bp, sp
    pushad

    mov bx, word [bp + 2 * 2]   ; bx = startSector[15:0]
    mov al, bl  ; al = startSector[7:0]
    mov dx, 0x1F3
    out dx, al
    mov al, bh  ; al = startSector[15:8]
    mov dx, 0x1F4
    out dx, al
    
    mov bx, word [bp + 3 * 2]   ; bx = startSector[27:16]
    and bx, 0x0FFF              ; bx[15:12] = 0
    or  bx, 0xE000              ; bx[15:12] = 0b1110 (LBA method)
    mov al, bl  ; al = startSector[23:16]
    mov dx, 0x1F5
    out dx, al
    mov al, bh  ; al = 0xE:startSector[27:24]
    mov dx, 0x1F6
    out dx, al

    mov al, byte [bp + 4 * 2]    ; al = sectorCount
    mov dx, 0x1F2
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

    .wait_disk:
        mov dx, 0x1F7
        in  al, dx
        and al, 0x88
        cmp al, 0x08
        jnz .wait_disk

    xor ax, ax
    mov al, byte [bp + 4 * 2]   ; ax = sectorCount
    imul ax, 256    ; ax = word count
    mov bx, word [bp + 5 * 2]   ; bx = targetAddress
    mov si, 0       ; shift = 0
    mov cx, ax      ; set loop = ax
    .write_word:
        mov dx, 0x1F0
        in  ax, dx
        mov word [bx + si], ax
        add si, 2
        loop .write_word

    popad
    pop sp
    ret

; Fill 0 before byte 447
times 446 - ($ - $$) db 0

; Fill 0 (temporary) for partition table
times 510 - ($ - $$) db 0

; Magic number
dw 0xAA55