section .text
bits 64
default rel
global saxpyAsm
saxpyAsm:
    ; set up frame for > 4 parameters
    push rbp
    mov rbp, rsp
    add rbp, 16

    ; rcx <- n
    ; xmm1 <- A
    ; r8 <- X pointer
    ; r9 <- Y pointer
    ; rbp+32 <- Z pointer

    ; rdx <- offset for indexing
    xor rdx, rdx

    ; rax <- Z pointer
    mov rax, [rbp+32]

    start:
    ; get X[i]
    movss xmm0, [r8 + rdx]
    ; multiply X[i] to A
    mulss xmm0, xmm1
    ; get Y[i]
    movss xmm2, [r9 + rdx]
    ; add A*X[i] to Y[i]
    addss xmm0, xmm2
    ; write sum to Z[i]
    movd [rax], xmm0
    add rdx, 4
    add rax, 4
    loop start

    pop rbp
    ret
