%macro EnterRealMode 0
    [bits 32]
    jmp word 0x18:.pm16

.pm16:
    [bits 16]

    mov eax, cr0
    and al, ~1
    mov cr0, eax

    jmp word 0x00:.rm

.rm:
    xor ax, ax
    mov ds, ax
    mov ss, ax

    sti

%endmacro

%macro EnterProtectedMode 0
    [bits 16]
    cli

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x08:.pm

.pm:
    [bits 32]
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

%endmacro

%macro LinearToSegment 4
    mov %3, %1
    shr %3, 4
    mov %2, %4
    mov %3, %1
    and %3, 0xF

%endmacro

global outbyte
outbyte:
    [bits 32]
    mov dx, [esp + 4] ; port
    mov al, [esp + 8] ; data
    out dx, al
    ret

global inbyte
inbyte:
    [bits 32]
    mov dx, [esp + 4] ; port
    xor eax, eax
    in al, dx ; take input into al (return value)
    ret

global disk_getparams
disk_getparams:
    [bits 32]

    push ebp
    mov ebp, esp

    EnterRealMode

    [bits 16]

    push es
    push bx
    push esi
    push di

    mov dl, [bp + 8]
    mov ah, 0x08

    xor di, di
    mov es, di

    stc
    int 0x13

    mov eax, 1
    sbb eax, 0

    LinearToSegment [bp + 12], es, esi, si
    mov [es:si], bl

    mov bl, ch
    mov bh, cl
    shr bh, 6
    inc bx

    LinearToSegment [bp + 16], es, esi, si
    
    mov [es:si], bx
    
    xor ch, ch
    and cl, 0x3F

    LinearToSegment [bp + 20], es, esi, si
    
    mov [es:si], cx
    mov cl, dh
    inc cx

    LinearToSegment [bp + 24], es, esi, si

    mov [es:si], cx

    pop di
    pop esi
    pop bx
    pop es

    push eax

    EnterProtectedMode

    [bits 32]

    pop eax

    mov esp, ebp
    pop ebp
    ret

global disk_reset
disk_reset:
    [bits 32]

    push ebp
    mov ebp, esp

    EnterRealMode

    [bits 16]

    mov ah, 0x00
    mov dl, [bp + 8]
    stc
    int 0x13

    mov eax, 1
    sbb eax, 0

    push eax

    EnterProtectedMode

    [bits 32]

    pop eax

    mov esp, ebp
    pop ebp
    ret

global disk_read
disk_read:
    push ebp
    mov ebp, esp

    EnterRealMode

    [bits 16]

    push ebx
    push es

    mov dl, [bp + 8]
    mov ch, [bp + 12]
    mov cl, [bp + 13]
    shl cl, 6

    mov al, [bp + 16]
    and al, 0x3F
    or cl, al

    mov dh, [bp + 20]
    mov al, [bp + 24]

    LinearToSegment [bp + 28], es, ebx, bx

    mov ah, 0x02
    stc
    
    int 0x13

    mov eax, 1
    sbb eax, 0

    pop es
    pop ebx

    push eax

    EnterProtectedMode

    [bits 32]

    pop eax

    mov esp, ebp
    pop ebp
    ret
