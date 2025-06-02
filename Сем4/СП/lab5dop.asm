.model tiny
.code
org 100h

start:
    mov cx, 10
    jmp main_loop

main_loop:
    call ClrScr
    mov ah, 09h
    lea dx, start_msg
    int 21h
    
    lea dx, input_buffer
    mov ah, 0Ah
    int 21h
    
    cmp buffer[0], '*'
    je exit_program
    
    mov bl, input_buffer[1]
    cmp bl, 0  
    je empty_input

    call NewLine
    
    xor bh, bh
    mov byte ptr buffer[bx], '$'
    
    mov ah, 09h
    lea dx, buffer
    int 21h
    
    mov ah, 09h
    lea dx, equals
    int 21h

    lea si, buffer              
    mov cl, input_buffer[1]     
    xor ch, ch
    call PrintHex               
    
    call WaitForKey             

    loop main_loop
    
empty_input:
    mov ah, 09h
    lea dx, no_input_msg
    int 21h
    call WaitForKey
    jmp main_loop
    
exit_program:
    call ClrScr
    
    mov ah, 09h
    lea dx, exit_msg
    int 21h
    
    mov ah, 4Ch
    mov al, 0
    int 21h

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
    lea si, buffer
    pop cx
    jcxz end_print

hex_loop:
    mov al, [si]
    cmp al, '$'                 
    je end_print
    cmp al, ' '                 
    je skip_char

    mov al, [si]
    shr al, 4
    call HexDigit

    mov al, [si]
    and al, 0Fh
    call HexDigit

    mov ah, 02h
    mov dl, ' '
    int 21h

skip_char:
    inc si
    loop hex_loop

end_print:
    ret
PrintHex endp

HexDigit proc
    and al, 0Fh                 
    cmp al, 10                  
    jb digit
    add al, 'A' - 10            
    jmp print
digit:
    add al, '0'                 
print:
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

start_msg      db 'Введите строку (завершение Enter, выход * в начале): $'
no_input_msg   db 'Пустой ввод$'
equals         db ' = $'
continue_msg   db 13, 10, 'Нажмите любую клавишу для продолжения...$'
exit_msg       db 'Программа успешно завершена!', 13, 10
               db 'Студент: Чернев Николай Андреевич', 13, 10
               db 'Группа: ИУ5-44Б', 13, 10
               db 'Вариант: 22', 13, 10, '$'

input_buffer   db 20            
               db ?             
buffer         db 20 dup(?)     

end start