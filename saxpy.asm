section .data
n dq 0
Xptr dq 0
Yptr dq 0
Zptr dq 0
tempfp dq 0 ; to transfer hex from register to xmm

section .text
global saxpyAsm
saxpyAsm:
    ; set up frame
    push rbp
    mov rbp, rsp
    add rbp, 16

    ; initialize parameters
    mov [n], rcx
    ; xmm1 <- A
    mov [Xptr], r8
    mov [Yptr], r9
    mov rax, [rbp+32]
    mov [Zptr], rax

    ; initialize idx
    mov rcx, 0

    start:
    ; while idx < ctr, continue
    cmp rcx, [n]
    jz end

    ; get &X[i], dereference, and move to variable
    mov rax, [Xptr]
    mov rax, [rax]
    mov [tempfp], rax

    ; move X[i] to xmm0
    movss xmm0, [tempfp]

    ; multiply X[i] (xmm0) to A (xmm1)
    mulss xmm0, xmm1

    ; get &Y[i] and dereference, and move to variable
    mov rax, [Yptr]
    mov rax, [rax]
    mov [tempfp], rax

    ; add Y[i] to A * X[i] (xmm0)
    addss xmm0, [tempfp]

    ; get &Z[i]
    mov rax, [Zptr]

    ; move function result (xmm0) to where Z[i] is pointing
    movss [tempfp], xmm0
    mov rdx, [tempfp]
    mov dword [rax], edx

    ; increment idx, pointers, then loop
    inc rcx
    add qword [Xptr], 4
    add qword [Yptr], 4
    add qword [Zptr], 4
    jmp start

    end:
    pop rbp
    ret