STACK_SEG SEGMENT PARA STACK 'STACK'
    db 100h dup(?)  
STACK_SEG ENDS

DATA_SEG SEGMENT PARA 'DATA'
        LET_A db 128
        LET_B db 129
        LET_C db 130
DATA_SEG ENDS



CODE_SEG SEGMENT PARA 'CODE'
    ASSUME CS:CODE_SEG, DS:DATA_SEG, SS:STACK_SEG
    main proc
        mov ax, DATA_SEG
        mov ds, ax

        mov dl, LET_A
        call PUTCH
        call CLRF

        mov dl, LET_B
        call PUTCH
        call CLRF

        mov dl, LET_C
        call PUTCH
        call CLRF

        call GETCH

        mov ah, 4Ch
        int 21h
    main endp

    PUTCH proc
        mov ah, 02h
        int 21h
        ret
    PUTCH endp

    CLRF proc
        mov ah, 02h
        mov dl, 0Ah 
        int 21h
        ret
    CLRF endp

    GETCH proc
        mov ah, 01h
        int 21h
        ret
    GETCH endp

CODE_SEG ENDS
    end main