%macro print 2
    mov rax, 1 
    mov rdi, 1
    mov rsi, %1
    mov rdx, %2 
    syscall 
%endmacro 
%macro read 2
    mov rax, 0
    mov rdi, 0
    mov rsi %1 
    mov rdx, %2 
    syscall 
%endmacro
%macro exit 0 
    mov rax, 60 
    mov rdi, 0 
    syscall 
%endmacro 

section .data 
    m1 db 10d, 13d, 'Enter Input number ", 10d, 13d 
    l1 equ $-m1 
    m2 db 10d, 13d, "Factorial of Number is: ", 10d, 13d 
    l2 equ $-m2 
    m3 db 10d, 13d, "Assignment No:9 To calculate factorial of number.", 10d, 13d 
    l3 equ $-m3 
    m4 db 10d, 13d, "======================================================", 10d, 13d 
    l4 equ $-m4 
    nline db 10 
    nline_len equ $-nline 

section .bss 
    numascii resb 16 
    factorial resq 1 
    answer resb 16 

section .text 
global _start 
_start:
    print m4, l4 
    print m3, l3 
    print m4, l4 
    print m1, l1 
    read numascii, 17 
    call asciihextohex:
    mov [factorial], rbx 
    mov rcx, [factorial] 
    call facro 
    mov rax, 00 
    print m2, l2 
    mov rax, qword[factorial]
    call display 
    print nline, nline_len 
    exit 

facto:
    push ecx 
    cmp rcx, 01 
    jne ahead 
    jmp exit2 
ahead: dec rcx 
    mov rax, rcx 
    mul qword[factorial] 
    mov qword[factorial], rax 
    call facto 
exit2:pop rcx 

    ret

asciihextohex:
    mov rsi, numascii 
    mov rcx, 16 
    mov rbx, 0 
    mov rax, 0

loop1: 
    rol rbx, 04 
    mov al, [rsi]
    cmp al, 39h 
    jbe skip1 
    sub al, 07h 
skip1: 
    sub al, 30h 
    add rbx, rax 
    inc rsi 
    dec rcx 
    jnz loop1 

    ret 

display: 
    mov rsi, andwer+15 
    mov rcx, 16 

loop2: 
    mov rdx, 0 
    mov rbx, 16 
    div rbx 
    cmp dl, 09h 
    jbe skip2 
    add dl, 07h 
skip2: 
    add dl, 30h 
    mov [rsi], dl 
    dec rsi 
    dec rcx 
    jns loop2 
    print answer, 16 
    ret 