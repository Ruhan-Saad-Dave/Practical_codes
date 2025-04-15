%macro read 2
mov Rax,0
mov Rdi,0
mov Rsi,%1
mov Rdx,%2
syscall
%endmacro
%macro print 2
mov Rax,1
mov Rdi,1
mov Rsi,%1
mov Rdx, %2
syscall
%endmacro
%macro exit 0
mov rax,60
mov rdi,0
syscall
%endmacro
section .data
msg db 10,"The array contents are are:",10
msg_len equ $-msg

section.bss
array resd 200
counter resb 1
section .text 
global_start 
_start:
;Accept numbers in an array
mov byte[counter],05
mov rsi,array
loop:
read rsi,17
add Rsi,17
dec byte[counter],05
mov rsi,array
loop1:
print rsi,17
add Rsi,17dec byte [counter]
jnz loop1

exit 
