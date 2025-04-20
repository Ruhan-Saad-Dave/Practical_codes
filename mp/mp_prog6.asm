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