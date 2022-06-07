[bits 32]

global writeSector
global readSector

writeSector:
    ; push addr
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
    mov al, 0x30
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
            mov ax, word [ebx + esi]
            out ax, dx
            add esi, 2
            loop .loop

readSector:
    ; push addr
    ; push sector
    ; push ret
    push ebp
    mov ebp, esp
    pushad

    mov ax, word [ebp + 2 * 4]; sector
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
        mov ebx, dword [ebp + 3 * 4]; addr
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