; print_int: Output signed integer in rax to stdout with newline
; Uses fixed 32-byte buffer, writes digits backwards to maintain stack alignment
; Handles negative numbers and zero cases

global print_int
section .text
print_int:
    PUSH rbp
    MOV rbp, rsp
    SUB rsp, 32              

    LEA r10, [rsp + 31]      
    MOV byte [r10], 10      
    DEC r10

    XOR rcx, rcx           
    MOV rbx, 10
    XOR r8d, r8d

    CMP rax, 0
    JE .print_zero
    JL .negative
    JMP .loop1

.negative:
    NEG rax
    MOV r8d, 1

.loop1:
    CQO
    IDIV rbx
    ADD rdx, '0'
    MOV byte [r10], dl
    DEC r10
    INC rcx
    CMP rax, 0
    JNE .loop1
    JMP .maybe_minus

.print_zero:
    MOV byte [r10], '0'
    DEC r10
    INC rcx

.maybe_minus:
    CMP r8d, 1
    JNE .print_call
    MOV byte [r10], '-'
    DEC r10
    INC rcx

.print_call:
    INC r10                  
    INC rcx                 

    MOV rax, 1
    MOV rdi, 1
    MOV rsi, r10           
    MOV rdx, rcx
    SYSCALL

    MOV rsp, rbp
    POP rbp
    RET
