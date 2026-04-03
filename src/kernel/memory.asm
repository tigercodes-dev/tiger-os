bits 32

global memcpy
memcpy:
    push ebp
    mov ebp, esp

    push edi
    push esi

    mov edi, [ebp + 8]
    mov esi, [ebp + 12]
    mov ecx, [ebp + 16]

    cld
    rep movsb

    mov eax, [ebp + 8]

    pop esi
    pop edi

    mov esp, ebp
    pop ebp
    ret

global memset
memset:
    push ebp
    mov ebp, esp

    push edi

    mov edi, [ebp + 8]
    mov al, [ebp + 12]
    mov ecx, [ebp + 16]

    cld
    rep stosb

    mov eax, [ebp + 8]

    pop edi

    mov esp, ebp
    pop ebp
    ret

global memcmp
memcmp:
    push ebp
    mov ebp, esp

    push esi
    push edi

    mov eax, 0

    mov esi, [ebp + 8]
    mov edi, [ebp + 12]
    mov ecx, [ebp + 16]

    repe cmpsb
    je .equal

    mov eax, 1

.equal:
    pop edi
    pop esi

    mov esp, ebp
    pop ebp
    ret
