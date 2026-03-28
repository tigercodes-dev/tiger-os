bits 32

global outbyte
outbyte:
    mov dx, [esp + 4] ; port
    mov al, [esp + 8] ; data
    out dx, al
    ret

global inbyte
inbyte:
    mov dx, [esp + 4] ; port
    xor eax, eax
    in al, dx ; take input into al (return value)
    ret
