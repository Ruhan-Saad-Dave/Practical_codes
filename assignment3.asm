%macro EFG 4

mov rax,%1
mov rdi,%2
mov rsi,%3
mov rdx,%4

syscall
%endmacro


%macro exit 0
mov Rax,60
mov Rdi,60
syscall
%endmacro

section .data 
array db 68h,28h,39h,00h,46h,9h,16h,22h
msg db 10,13,"the largest number in array is :",10,13
msg_len equ $-msg

section .bss
cnt resb 1
result resb 16

section .text
global _start
_start:
mov byte[cnt],08
mov rsi,array
mov al,0
lp1:
cmp al,[rsi]
jg skip
xchg al,[rsi]
skip:inc rsi
dec byte[cnt]
jnz lp1
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
skip2:add dl,30h
mov[rsi],dl
dec rsi
dec rcx
jnz loop2
EFG 1,1,msg,msg_len
EFG 1,1,result,16
ret
