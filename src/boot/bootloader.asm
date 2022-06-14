%include "boot.inc"
org 0x7E00
[bits 16]

; Set global descriptors

; Descriptor #0: Empty descriptor
mov dword [GDT_START_ADDR + 0 * 4], 0x00000000
mov dword [GDT_START_ADDR + 1 * 4], 0x00000000

; Descriptor #1: Code descriptor
mov dword [GDT_START_ADDR + 2 * 4], 0x0000FFFF
mov dword [GDT_START_ADDR + 3 * 4], 0x00CF9800

; Descriptor #2: Data descriptor
mov dword [GDT_START_ADDR + 4 * 4], 0x0000FFFF
mov dword [GDT_START_ADDR + 5 * 4], 0x00CF9200

; Descriptor #3: Stack descriptor
mov dword [GDT_START_ADDR + 6 * 4], 0x00000000
mov dword [GDT_START_ADDR + 7 * 4], 0x00409600

; Descriptor #4: Video Memory descriptor
mov dword [GDT_START_ADDR + 8 * 4], 0x80007FFF
mov dword [GDT_START_ADDR + 9 * 4], 0x0040920B

; Init GDTR
mov word [pgdt], 39
lgdt [pgdt]

; Open A20 gate
in  al, 0x92
or  al, 0000_0010B
out 0x92, al

; Set PE
cli                         ; Clear interrupt enable bit
mov eax, cr0
or  eax, 1
mov cr0, eax

; Jump to protected mode
jmp dword CODE_SELECTOR:protect_mode

; Halt
jmp $

[bits 32]
protect_mode:
    ; Load selectors
    mov eax, DATA_SELECTOR
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov eax, STACK_SELECTOR
    mov ss, eax

    ; Load kernel loader
    xor eax, eax
    mov eax, KERNEL_LOADER_SECTOR_START
    mov ebx, KERNEL_LOADER_START_ADDR
    mov ecx, KERNEL_LOADER_SECTOR_COUNT
    .load_kernel_loader:
        push ebx
        push eax
        call asm_read_sector_pm
        add esp, 8
        inc eax
        add ebx, 512
        loop .load_kernel_loader

; Jump to kernel
jmp CODE_SELECTOR:KERNEL_LOADER_START_ADDR

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
    mov dx, 0x1F3
    out dx, al
    mov al, ah
    inc dx
    out dx, al
    mov ax, word [ebp + 2 * 4 + 2]
    inc dx
    out dx, al
    and ax, 0x0FFF
    or  ax, 0xE000
    mov al, ah
    inc dx
    out dx, al
    mov al, 0x01
    mov dx, 0x1F2
    out dx, al
    mov al, 0x20
    mov dx, 0x1F7
    out dx, al

    .wait:
        mov dx, 0x1F7
        in  al, dx
        and al, 0x88
        cmp al, 0x08
        jnz .wait
    
    .write:
        mov ebx, dword [ebp + 3 * 4]
        mov ecx, 256
        mov esi, 0
        mov dx, 0x1F0
        .loop:
            in  ax, dx
            mov word [ebx + esi], ax
            add esi, 2
            loop .loop
    
    popad
    pop ebp
    ret

; GDT Inf
pgdt dw 0
     dd GDT_START_ADDR
