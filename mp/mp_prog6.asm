section .data
    rmodemsg db 10, 'Peocessor is in Real Mode'
    rmsg_len:equ $-rmodemsg

    pmodemsg db 10, 'Processor is in Protected Mode'
    pmsg_len:equ $-pmodemsg 

    gdtmsg db 10, 'GDT Contents are::'
    gmsg_len:equ $-gdtmsg 

    ldtmsg db 10, 'LDT Contents are ::'
    lmsg_len:equ $-ldtmsg 

    idtmsg db 10, 'IDT Contents are ::'
    imsg_len:equ $-idtmsg 

    trmsg db 10, 'Task Register COntents are::'
    tmsg_len: equ $-trmsg

    mswmsg db 10, 'Machine Status Word::''
    mmsg_len: equ $-mswmsg

    colmsg db ':'

    nwline db 10

section .bss
    gdt resd 1
        resw 1
    ldt resw 1
    idt resd 1
        resw 1
    tr resw 1

    cr0_data resd 1 

    dnum_buff resb 04 

%macro disp 2
    mov rax, 1
    mov rdi, 1
    mov rsi, %1
    mov rdx, %2
    syscall
%endmacro 

section .text 
    global _start
_start: 
    smsw rax 

    mov [cro_data], rax 

    bt eax, 0

    jc prmode
    disp rmodemsg, rmsg_len 
    jmp nxt1 

prmode: disp pmodemsg, pmsg_len
nxt1: sgdt [gdt]
    sldt [ldt]
    sidt [idt]
    str [tr]

    disp gdtmsg, gmsg_len
    mov bx, [gdt+4]
    call disp_num

    mov bx, [gdt+2]
    call disp_num 

    disp colmsg, 1

    mov bx, [gdt]
    call disp_num

    disp ldtmsg, lmsg_len 
    mov bx, [ldt]
    call disp_num 

    disp idtmsg, imsg_len 

    mov bx, [idt+4]
    call disp_num 

    mov bx, [idt+2]
    call disp_num
    disp colmsg, 1

    mov bx, [idt]
    call disp_num
    disp trmsg, tmsg_len
    mov bx, [tr]
    call disp_num 

    disp trmg, tmsg_len 

    mov bx, [tr] 
    call disp_num 

    disp mswmsg, mmsg_len 

    mov bx, [cr0_data + 2]
    call disp_num 

    mov bx, [cr0_data]
    call disp_num 

    disp nwline, 1
exit: mov eax, 01
    mov rbx, 00
    int 80h 

disp_num:
    mov esi, dnum_buff 
    
    mov ecx, 04 

up1:
    rol bx, 4
    mov dl, bl 
    and dl, 0fh 
    add dl, 30h 
    cmp dl, 39h 
    jbe skip1 
    add dl, 07h
skip1:
    mov [esi], dl
    inc esi 
    loop up1 

    disp dnum_buff, 4

    ret
