bits 32

global outbyte
outbyte:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    mov al, [ebp + 12] ; data
    out dx, al

    mov esp, ebp
    pop ebp
    ret

global inbyte
inbyte:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    xor eax, eax
    in al, dx ; take input into al (return value)

    mov esp, ebp
    pop ebp
    ret

global outword
outword:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    mov ax, [ebp + 12] ; value
    out dx, ax

    mov esp, ebp
    pop ebp
    ret

global inword
inword:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    xor eax, eax
    in ax, dx

    mov esp, ebp
    pop ebp
    ret

global outdword
outdword:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    mov eax, [ebp + 12] ; value
    out dx, eax

    mov esp, ebp
    pop ebp
    ret

global indword
indword:
    push ebp
    mov ebp, esp

    mov dx, [ebp + 8] ; port
    xor eax, eax
    in eax, dx

    mov esp, ebp
    pop ebp
    ret
