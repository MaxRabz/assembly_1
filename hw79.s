.text

    .globl relation_code
    .type relation_code, @function
relation_code:
    # TODO: Problem 1
    movq $0, %rax
    cmpq %rsi, %rdi
    jl signed_less
    jb unsigned_less
    ret
signed_less:
    addq $1, %rax
    cmpq %rsi, %rdi
    jb unsigned_less
    ret
unsigned_less:
    addq $2, %rax
    ret

    .globl clamp_long
    .type clamp_long, @function
clamp_long:
    # TODO: Problem 2
    movq $0, %rax
    cmpq %rsi, %rdi
    jl low
    cmpq %rdx, %rdi
    jg high
    movq %rdi, %rax
    ret
low:
    movq %rsi, %rax
    ret
high:
    movq %rdx, %rax
    ret

    .globl weighted_sum8
    .type weighted_sum8, @function
weighted_sum8:
    # TODO: Problem 3
    movq %rdi, %rax
    imulq $2, %rsi, %rdi
    addq %rdi, %rax
    imulq $3, %rdx, %rdi
    addq %rdi, %rax
    imulq $4, %rcx, %rdi
    addq %rdi, %rax
    imulq $5, %r8, %rdi
    addq %rdi, %rax
    imulq $6, %r9, %rdi
    addq %rdi, %rax
    movq 8(%rsp), %rdi
    imulq $7, %rdi, %rdi
    addq %rdi, %rax
    movq 16(%rsp), %rdi
    imulq $8, %rdi, %rdi
    addq %rdi, %rax 
    ret

    .globl alternating_sum
    .type alternating_sum, @function
alternating_sum:
    # TODO: Problem 4
    movq $0, %rax
    movq $0, %rsi
    movq $0, %rdx
    cmpq %rsi, %rdi
    jle zero_case
repeat_case:
    addq $1, %rsi
    cmpq %rdi, %rsi
    jg end_case
    cmpq $0, %rdx
    je add_case
    jmp subtract_case
add_case:
    addq %rsi, %rax
    movq $1, %rdx
    jmp repeat_case
subtract_case:
    subq %rsi, %rax
    movq $0, %rdx
    jmp repeat_case
end_case:
    ret
zero_case:
    ret

    .globl recursive_power
    .type recursive_power, @function
recursive_power:
    # TODO: Problem 5
    pushq %rbp
    movq %rsp, %rbp
    movq $1, %rax
    cmpq $0, %rsi
    je base_case
    subq $8, %rsp
    movq %rdi, -8(%rbp)
    subq $1, %rsi
    call recursive_power
    imulq -8(%rbp), %rax
base_case:
    leave
    ret

    .globl combine_helpers
    .type combine_helpers, @function
combine_helpers:
    # TODO: Problem 6
    movq $0, %rax
    ret

    .section .note.GNU-stack,"",@progbits
