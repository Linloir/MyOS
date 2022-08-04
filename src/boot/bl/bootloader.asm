%include "boot.inc"
[org 0x7E00]
[bits 16]

; Disable interrupts
cli

; Initialize Global Descriptor Table
; 0x0 Empty selector
mov dword [GDT_START_ADDRESS + 0 * 4], 0x00000000
mov dword [GDT_START_ADDRESS + 1 * 4], 0x00000000

; 0x08 Kernel code selector
mov dword [GDT_START_ADDRESS + 2 * 4], 0x0000FFFF
mov dword [GDT_START_ADDRESS + 3 * 4], 0x00CF9A00

; 0x10 Kernel data selector
mov dword [GDT_START_ADDRESS + 4 * 4], 0x0000FFFF
mov dword [GDT_START_ADDRESS + 5 * 4], 0x00CF9200

; 0x18 Kernel stack selector
mov dword [GDT_START_ADDRESS + 6 * 4], 0x0000FFFF
mov dword [GDT_START_ADDRESS + 7 * 4], 0x00CF9600

; Set table size
mov word [gdt_descriptor], GDT_SIZE * 8 - 1

; Load gdt
lgdt [gdt_descriptor]

; Open A20
in  al, 0x92
or  al, 0b0000_0010
out 0x92, al

; Set PE
mov eax, cr0
or  eax, 1
mov cr0, eax

jmp dword KERNEL_CODE_SELECTOR:protected_mode_begin

; Halt here
jmp $

; Variables
gdt_descriptor  dw 0,
                dd GDT_START_ADDRESS

[bits 32]

protected_mode_begin:

; Set selectors
mov ax, KERNEL_DATA_SELECTOR
mov ds, ax
mov ax, KERNEL_STACK_SELECTOR
mov ss, ax
mov ax, ZERO_SELECTOR
mov es, ax
mov fs, ax
mov gs, ax

; Read kernelloader
push KERNELLOADER_LOAD_ADDRESS
push KERNELLOADER_SECTOR_COUNT
push KERNELLOADER_SECTOR_START
call read_sectors
add esp, 12

; Jump to kernelloader
jmp KERNELLOADER_LOAD_ADDRESS

; Halt here
jmp $

; Loader sector function
; Notice: Loads using LBA method
read_sectors:
    ; push targetAddr
    ; push sectorCount
    ; push startSector[27:0]
    ; push ret
    push esp
    mov ebp, esp
    pushad

    mov bx, word [ebp + 2 * 4]   ; bx = startSector[15:0]
    mov al, bl  ; al = startSector[7:0]
    mov dx, 0x1F3
    out dx, al
    mov al, bh  ; al = startSector[15:8]
    mov dx, 0x1F4
    out dx, al
    
    mov bx, word [ebp + 2 * 4 + 2]   ; bx = startSector[27:16]
    and bx, 0x0FFF              ; bx[15:12] = 0
    or  bx, 0xE000              ; bx[15:12] = 0b1110 (LBA method)
    mov al, bl  ; al = startSector[23:16]
    mov dx, 0x1F5
    out dx, al
    mov al, bh  ; al = 0xE:startSector[27:24]
    mov dx, 0x1F6
    out dx, al

    mov al, byte [ebp + 3 * 4]    ; al = sectorCount
    mov dx, 0x1F2
    out dx, al

    mov dx, 0x1F7
    mov al, 0x20
    out dx, al

    xor ecx, ecx
    mov cl, byte [ebp + 3 * 4]  ; cx = sectorCount
    mov ebx, dword [ebp + 4 * 4]   ; bx = targetAddress
    mov esi, 0       ; shift = 0
    .read_one_sec:
        .wait_disk:
            mov dx, 0x1F7
            in  al, dx
            and al, 0x89
            cmp al, 0x08
            jnz .wait_disk

        push ecx
        mov ecx, 256    ; read one sector
        .write_word:
            mov dx, 0x1F0
            in  ax, dx
            mov word [ebx + esi], ax
            add esi, 2
            loop .write_word
        pop ecx
        loop .read_one_sec

    popad
    pop esp
    ret
