%macro scall 4
mov rax,%1
mov rdi,%2
mov rsi,%3
mov rdx,%4
syscall
%endmacro

section .data
      arr dq 722222221111111h,-111111100000000h,-7999999999999999h,7FFFFFFFFFFFFFFFh
      n equ 4
      pmsg db 10d,13d,"the count of positive no:",10d,13d
      plen equ $-pmsg
      nmsg db 10d,13d,"the count of negative no:",10d,13d
      nlen equ $-nmsg
      nwline db 10d,13d

section .bss
pcnt resq 1
ncnt resq 1
char_answer resb 16

section .text
global _start
_start:
      mov rsi,arr
      mov rdi,n
      mov rbx,0
      mov rcx,0
up: mov rax,[rsi]
    cmp rax, 0000000000000000h
    js negative 
    positive:inc rbx
jmp next
negative: inc rcx

next:add rsi,8
     dec rdi
     jnz up
     mov [pcnt],rbx
     mov [ncnt],rcx
     scall 1,1,pmsg,plen
    mov rax,[pcnt]
    call display
scall 1,1,nmsg,nlen
mov rax,[ncnt]
call display

scall 1,1,nwline,1
mov rax,60
mov rbx,0
syscall
display:
mov rsi,char_answer+15
mov rcx,16
cnt:mov rdx,0
mov rbx,16h
div rbx
cmp dl,09h
jbe add30
add dl,07h
add30:add dl,30h
mov [rsi],dl
dec rsi
dec rcx
jnz cnt

scall 1,1,char_answer,16
ret


