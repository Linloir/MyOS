%include "boot.inc"
org 0x7E00
[bits 16]

; Set global descriptors

; Descriptor #0: Empty descriptor
mov dword [GDT_START_ADDR + 0 * 4], 0x00000000
mov dword [GDT_START_ADDR + 1 * 4], 0x00000000

; Descriptor #1: Data descriptor
mov dword [GDT_START_ADDR + 2 * 4], 0x0000FFFF
mov dword [GDT_START_ADDR + 3 * 4], 0x00CF9200

; Descriptor #2: Code descriptor
mov dword [GDT_START_ADDR + 4 * 4], 0x0000FFFF
mov dword [GDT_START_ADDR + 5 * 4], 0x00DC9200

; Descriptor #3: Stack descriptor
mov dword [GDT_START_ADDR + 6 * 4], 0x0000000F
mov dword [GDT_START_ADDR + 7 * 4], 0x00C09600

; Descriptor #4: Video Memory descriptor
mov dword [GDT_START_ADDR + 8 * 4], 0x80007FFF
mov dword [GDT_START_ADDR + 9 * 4], 0x0040920B

; Init GDTR
mov word [pgdt], 39
lgdt [pgdt]

; Open A20 gate
in  al, 0x92
or  al, 0b0000_0010
out 0x92, al

; Set PE
cli                         ; Clear interrupt enable bit
mov eax, cr0
or  eax, 1
mov cr0, eax

; Jump to protected mode
jmp CODE_SELECTOR:protect_mode

; Halt
jmp $

; GDT Inf
pgdt:
    dw 0
    dd GDT_START_ADDR

[bits 32]
protect_mode:
    ; Load selectors
    mov eax, DATA_SELECTOR
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov eax, STACK_SELECTOR
    mov ss, eax
    mov eax, VIDEO_SELECTOR
    mov gs, eax

    ; Load kernel
    mov eax, KERNEL_SECTOR_START
    or  eax, 0xE0000000
    mov ebx, KERNEL_START_ADDR
    mov ecx, KERNEL_SECTOR_COUNT
    .load_kernel:
        push ebx
        push eax
        call asm_read_sector_pm
        add esp, 8
        add eax, 1
        add ebp, 512

; Jump to kernel
jmp CODE_SELECTOR:KERNEL_START_ADDR

; Halt
jmp $

asm_read_sector_pm:
    ; push readToAddr
    ; push sector
    ; push ret
    push ebp
    mov ebp, esp
    pushad

    mov ax, word [ebp + 2 * 4]
    out 0x1F3, al
    out 0x1F4, ah
    mov ax, word [ebp + 2 * 4 + 2]
    out 0x1F5, al
    out 0x1F6, ah
    out 0x1F2, 0x01
    out 0x1F7, 0x20

    .wait:
        in  al, 0x1F7
        and al, 0x88
        cmp al, 0x08
        jnz .wait
    
    .write:
        mov ebx, dword [ebp + 3 * 4]
        mov ecx, 256
        mov esi, 0
        .loop:
            in  ax, 0x1F0
            mov dword [ebx + esi], ax
            add esi, 2
    
    popad
    pop ebp
    ret