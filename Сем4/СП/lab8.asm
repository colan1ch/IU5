.model small
.stack 100h

.data
    msg1        db 'Вывод значений регистров AX и CS:$'
    msg2        db 'Вывод дампа памяти по заданному адресу:$'
    prompt      db 'Введите адрес (формат: НННН:НННН)>$'
    error_msg   db 'Ошибка! Введите корректный адрес!$'
    exit_msg    db 'Программа завершена успешно!$'
    hx          db 4 dup (32), '$'
    byte_buf    db 2 dup ('0'), ' $'
    char_buf    db ' $'
    segment_addr dw 0
    offset_addr  dw 0
    end_offset   dw 0
    hex_chars    db '0123456789ABCDEF'
    regAX       db 'AX=$'
    regCS       db ' CS=$'
    colon       db ':$'
    space       db ' $'
    newline     db 0Dh, 0Ah, '$'
    input_buf   db 9, ?, 9 dup(0)  ; Буфер для ввода адреса (8 символов)

.code
start:
    mov ax, @data
    mov ds, ax
    
    ; Вывод регистров AX и CS
    call clear_screen
    mov dx, offset msg1
    call print_string
    call new_line
    
    mov dx, offset regAX
    call print_string
    mov ax, ax      ; Текущее значение AX
    call print_hex
    mov dx, offset regCS
    call print_string
    mov ax, cs
    call print_hex
    call new_line
    
    ; Ввод адреса и вывод дампа
input_loop:
    call clear_screen
    mov dx, offset msg2
    call print_string
    call new_line
    
    mov dx, offset prompt
    call print_string
    
    ; Ввод адреса
    mov dx, offset input_buf
    mov ah, 0Ah
    int 21h
    
    ; Проверка на выход
    mov al, input_buf+2
    cmp al, '*'
    je exit_program
    
    ; Разбор адреса
    call parse_address
    jnc dump_memory
    
    ; Ошибка ввода
    mov dx, offset error_msg
    call print_string
    call wait_key
    jmp input_loop
    
dump_memory:
    call clear_screen
    
    ; Вывод информации об адресе
    mov dx, offset msg2
    call print_string
    call new_line
    
    mov dx, offset regAX
    call print_string
    mov ax, ax      ; Текущее значение AX
    call print_hex
    mov dx, offset regCS
    call print_string
    mov ax, cs
    call print_hex
    call new_line
    
    ; Настройка сегмента
    mov ax, segment_addr
    mov es, ax
    mov si, offset_addr
    
    ; Вывод 16 строк по 24 байта
    mov cx, 16
dump_lines:
    push cx
    
    ; Вывод адреса строки
    mov ax, es
    call print_hex
    mov dx, offset colon
    call print_string
    mov ax, si
    call print_hex
    mov dx, offset space
    call print_string
    
    ; Вывод 24 байт в hex
    mov cx, 24
dump_bytes_hex:
    mov al, es:[si]
    call print_byte
    inc si
    loop dump_bytes_hex
    
    ; Возврат к началу строки для символьного вывода
    sub si, 24
    
    ; Вывод 24 байт в символьном виде
    mov dx, offset space
    call print_string
    mov dl, '|'
    call print_char
    mov cx, 24
dump_bytes_char:
    mov al, es:[si]
    cmp al, 20h
    jb non_printable
    cmp al, 7Eh
    ja non_printable
    mov dl, al
    jmp print_char_now
non_printable:
    mov dl, '.'
print_char_now:
    call print_char
    inc si
    loop dump_bytes_char
    
    mov dl, '|'
    call print_char
    call new_line
    
    pop cx
    loop dump_lines
    
    ; Ожидание Enter или *
    call wait_key
    cmp al, 0Dh    ; Enter
    je next_page
    cmp al, '*'
    jne dump_memory
    
next_page:
    ; Переход к следующей странице
    mov ax, offset_addr
    add ax, 16*24
    mov offset_addr, ax
    jmp input_loop
    
exit_program:
    call clear_screen
    mov dx, offset exit_msg
    call print_string
    mov ax, 4C00h
    int 21h

; Процедуры
clear_screen proc
    mov ax, 0600h
    mov bh, 07h
    xor cx, cx
    mov dx, 184Fh
    int 10h
    mov ah, 02h
    xor bh, bh
    xor dx, dx
    int 10h
    ret
clear_screen endp

print_string proc
    mov ah, 09h
    int 21h
    ret
print_string endp

print_char proc
    mov ah, 02h
    int 21h
    ret
print_char endp

new_line proc
    mov dx, offset newline
    call print_string
    ret
new_line endp

wait_key proc
    mov ah, 08h
    int 21h
    ret
wait_key endp

print_hex proc
    push bx
    push cx
    push dx
    
    mov cx, 4
hex_out_loop:
    rol ax, 4
    mov bx, ax
    and bx, 0Fh
    mov dl, [hex_chars + bx]
    call print_char
    loop hex_out_loop
    
    pop dx
    pop cx
    pop bx
    ret
print_hex endp

print_byte proc
    push ax
    push dx
    
    mov dl, al
    shr dl, 4
    call print_nibble
    mov dl, al
    and dl, 0Fh
    call print_nibble
    mov dl, ' '
    call print_char
    
    pop dx
    pop ax
    ret
print_byte endp

print_nibble proc
    cmp dl, 10
    jb digit
    add dl, 'A' - 10
    jmp print
digit:
    add dl, '0'
print:
    call print_char
    ret
print_nibble endp

parse_address proc
    ; Проверка длины ввода (должно быть 8 символов)
    mov al, input_buf+1
    cmp al, 8
    jne parse_error
    
    ; Проверка наличия ':'
    cmp input_buf+6, ':'
    jne parse_error
    
    ; Преобразование сегмента
    mov si, offset input_buf+2
    mov cx, 4
    call hex_to_num
    jc parse_error
    mov segment_addr, ax
    
    ; Преобразование смещения
    mov si, offset input_buf+7
    mov cx, 4
    call hex_to_num
    jc parse_error
    mov offset_addr, ax
    
    ; Вычисление конечного смещения
    mov ax, offset_addr
    add ax, 16*24 - 1
    mov end_offset, ax
    
    clc
    ret
    
parse_error:
    stc
    ret
parse_address endp

hex_to_num proc
    xor ax, ax
    xor bx, bx
hex_loop:
    shl ax, 4
    mov bl, [si]
    inc si
    
    cmp bl, '0'
    jb hex_error
    cmp bl, '9'
    jbe digit2
    and bl, 0DFh
    cmp bl, 'A'
    jb hex_error
    cmp bl, 'F'
    ja hex_error
    
digit2:
    cmp bl, '9'
    jbe digit_num
    sub bl, 'A' - 10
    jmp add_digit
digit_num:
    sub bl, '0'
add_digit:
    add ax, bx
    loop hex_loop
    clc
    ret
hex_error:
    stc
    ret
hex_to_num endp

end start