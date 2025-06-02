STACKSEG SEGMENT STACK
    DB 128 DUP(?)
STACKSEG ENDS

DATAS SEGMENT
    msg_exit db 13, 10, 'Program finished.$'
DATAS ENDS

CODES SEGMENT
ASSUME CS:CODES, DS:DATAS, SS:STACKSEG

start:
    mov ax, DATAS
    mov ds, ax
    mov ax, STACKSEG
    mov ss, ax

main_loop:
    call GETCH

    cmp al, '*'
    je exit_program

    push ax
    
    mov cx, 10

print_loop:
    call PUTCH
    
    call CLRF

    pop ax
    dec al
    dec al
    push ax 

    loop print_loop

    pop ax
    call CLRF

    jmp main_loop

GETCH proc
    mov ah, 08h
    int 21h
    ret
GETCH endp

PUTCH proc
    mov dl, al
    mov ah, 02h
    int 21h
    ret
PUTCH endp

CLRF proc
    mov al, 13
    call PUTCH
    mov al, 10
    call PUTCH
    ret
CLRF endp


exit_program:
    mov ah, 09h
    lea dx, msg_exit
    int 21h

    mov ax, 4C00h
    int 21h

CODES ENDS
END start