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

; Print something
xor eax, eax
mov ebx, 0xB8000
mov esi, 0
print:
    mov dl, byte [_msg + esi]
    cmp dl, 0
    je  print_exit
    inc esi
    ; Calculate cordinate in vga memory
    mov eax, dword [_row]
    imul eax, 80
    add eax, dword [_col]
    imul eax, 2
    ; Copy character to memory
    mov edi, eax
    mov byte [ebx + edi], dl    ; Character to be printed
    mov byte [ebx + edi + 1], 0x0F  ; Black background with white foreground
    ; Set new cordinate
    add dword [_col], 1
    mov eax, dword [_col]
    cmp eax, 80
    jne add_row_exit
    add dword [_row], 1
    mov dword [_col], 0
    add_row_exit:
    ; Reture to top
    jmp print
print_exit:

; Halt here
jmp $

; Variables
_row    dd 0
_col    dd 0
_msg    db 'Hello World!',
        db 0
