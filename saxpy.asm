section .data
header db "SAXPY results in Assembly (first 10)",10,10,0
loop_msg db "Z_Asm[%d] = %f",10,0
section .text
bits 64
default rel
global saxpyAsm
extern printf
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

    ; print header message
    sub rsp, 8*6
    mov rcx, header
    call printf
    add rsp, 8*6

    ; initialize loop
    ; rdx <- counter/index
    mov rdx, 0
    ; r10 <- Z pointer
    mov r10, [rbp+32]

    print_results:
    ; assume there are at least 10 elements to print out
    cmp rdx, 10
    jz end

    ; get and convert Z[i] to double as %f accepts doubles, not floats
    movd xmm0, [r10]
    cvtss2sd xmm0, xmm0

    ; preserve rdx and r10
    push r10
    push rdx

    ; print values
    sub rsp, 8*6
    movq r8, xmm0
    ; rdx already has counter/index
    mov rcx, loop_msg
    call printf
    add rsp, 8*6

    ; get back rdx and r10
    pop rdx
    pop r10

    inc rdx
    add r10, 4
    jmp print_results

    end:
    pop rbp
    ret
