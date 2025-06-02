.model tiny
.code
org 100h

start:
    mov cx, 10
    jmp main_loop_start

main_loop_start:
main_loop:
    call ClrScr
    mov ah, 09h
    lea dx, start_msg
    int 21h
    lea di, buffer
    xor bx, bx

input_loop:
    call GetChNoEcho


    cmp al, '*'
    jne not_exit_char
    cmp bx, 0
    je exit_program
    
not_exit_char:
    cmp al, 08h
    jne not_backspace
    
    cmp bx, 0
    je input_loop
    
    dec di
    dec bx
    
    mov ah, 02h
    mov dl, 08h
    int 21h
    mov dl, ' '
    int 21h
    mov dl, 08h
    int 21h
    
    jmp input_loop
    
not_backspace:
    cmp al, 0Dh
    je end_input
    
    cmp al, '$'
    je end_input

    cmp bx, 19
    jae input_loop

    mov [di], al
    inc di
    inc bx
    
    mov dl, al
    mov ah, 02h
    int 21h
    
    jmp input_loop

end_input:
    mov byte ptr [di], '$'
    inc bx

    call NewLine

    lea si, buffer
    mov cx, bx
    call PrintHex

    call WaitForKey

    dec cx
    jnz main_loop
    jmp exit_program

exit_program:
    call ClrScr
    
    mov ah, 09h
    lea dx, exit_msg
    int 21h
    
    mov ah, 4Ch
    mov al, 0
    int 21h


GetChNoEcho proc
    mov ah, 08h
    int 21h
    ret
GetChNoEcho endp


GetCh proc
    mov ah, 01h
    int 21h
    ret
GetCh endp


PutCh proc
    mov dl, al
    mov ah, 02h
    int 21h
    ret
PutCh endp


NewLine proc
    mov ah, 02h
    mov dl, 0Dh
    int 21h
    mov dl, 0Ah
    int 21h
    ret
NewLine endp


ClrScr proc
    mov ax, 0600h
    mov bh, 07h
    mov cx, 0000h
    mov dx, 184Fh
    int 10h
    
    mov ah, 02h
    mov bh, 0
    mov dx, 0000h
    int 10h
    ret
ClrScr endp


PrintHex proc
    push cx

    mov ah, 09h
    lea dx, buffer
    int 21h

    mov ah, 09h
    lea dx, equals
    int 21h

    lea si, buffer
    pop cx
    dec cx

hex_output:
    mov al, [si]

    cmp al, ' '
    je skip_space

    xor ah, ah
    push ax

    mov al, byte ptr [si]
    shr al, 4
    call HexDigit

    mov al, byte ptr [si]
    and al, 0Fh
    call HexDigit

    pop ax

    push ax
    mov ah, 02h
    mov dl, ' '
    int 21h
    pop ax

skip_space:
    inc si
    loop hex_output

    call NewLine
    ret
PrintHex endp


; Hex proc
;     push ax
;     push dx

;     mov dl, al

;     mov al, dl
;     shr al, 4
;     and al, 0Fh
;     call HexDigit

;     mov al, dl
;     and al, 0Fh
;     call HexDigit

;     pop dx
;     pop ax
;     ret
; Hex endp


HexDigit proc
    and al, 0Fh
    
    cmp al, 10
    jb digit
    
    add al, 'A' - 10
    jmp output_hex
    
digit:
    add al, '0'
    
output_hex:
    mov dl, al
    mov ah, 02h
    int 21h
    ret
HexDigit endp


WaitForKey proc
    mov ah, 09h
    lea dx, continue_msg
    int 21h
    
    mov ah, 08h
    int 21h
    ret
WaitForKey endp


start_msg  db 'Введите строку (завершение Enter, выход * в начале): $'
equals      db ' = $'
continue_msg db 13, 10, 'Нажмите любую клавишу для продолжения...$'
exit_msg    db 'Программа успешно завершена!', 13, 10
            db 'Студент: Чернев Николай Андреевич', 13, 10
            db 'Группа: ИУ5-44Б', 13, 10
            db 'Вариант: 22', 13, 10, '$'

buffer      db 21 dup(?)

end start


