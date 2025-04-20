%macro read 2
mov Rax, 0
mov Rdi, 0
mov Rsi, %1
mov Rdx, %2
syscall
%endmacro

%macro print 2
mov Rax, 1
mov Rdi, 1
mov Rsi, %1
mov Rdx, %2
syscall
%endmacro

%macro exit 0
mov rax,60
mov rdi,0
syscall
%endmacro

section .data
msg db 10,13,"length of the string is:",10,13
msg_len equ $-msg

section .bss
str1 resb 200
result resb 17

section .text
global _start
_start:
read str1,200
call display
exit

display:
mov rsi,result+15
mov rcx,16
loop2:
mov rdx,0
mov rbx,16
div rbx
cmp dl,09h
jbe skip2
add dl,07h
skip2:
add dl,30h
mov [rsi],dl
dec rsi
dec rcx
jnz loop2
print msg,msg_len
print result,16
ret

