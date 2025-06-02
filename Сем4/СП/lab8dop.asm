; HEX Memory Dump ? надёжный, без зацикливания
cseg segment
assume cs:cseg, ds:cseg
org 100h

start:
    jmp begin

hex_digits db '0123456789ABCDEF'
prompt db 'Введите адрес (сегмент:смещение)>$'
equals db ' = $'
newline db 13,10,'$'

segment_addr dw 0
offset_addr dw 0

begin:
    push cs
    pop ds

    ; Ввод адреса
    mov dx, offset prompt
    mov ah, 09h
    int 21h

    call get_hex_word
    mov segment_addr, ax

    ; Печать ':'
    mov dl, ':'
    mov ah, 02h
    int 21h

    call get_hex_word
    mov offset_addr, ax

    call CLRF
    call dump_memory

    ; Выход
    mov ax, 4C00h
    int 21h

; -------------------------------------------------------------
; Процедура: dump_memory ? выводит 5 строк по 8 байт
; -------------------------------------------------------------
dump_memory proc
    push ds
    mov ax, segment_addr
    mov es, ax
    mov si, offset_addr

    mov bx, 5          ; line_count

next_line:
    ; segment:offset
    mov ax, segment_addr
    call print_hex_word
    mov dl, ':'
    mov ah, 02h
    int 21h
    mov ax, si
    call print_hex_word

    ; Пробел
    mov dl, ' '
    mov ah, 02h
    int 21h

    ; HEX байты
    push si
    mov cx, 8
print_hex_bytes:
    mov al, byte ptr es:[si]
    call print_hex_byte
    mov dl, ' '
    mov ah, 02h
    int 21h
    inc si
    loop print_hex_bytes

    ; Знак "="
    mov dx, offset equals
    mov ah, 09h
    int 21h

    ; ASCII
    pop si
    mov cx, 8
print_ascii:
    mov al, byte ptr es:[si]
    cmp al, 32
    jb not_print
    cmp al, 126
    ja not_print
    jmp ok_print
not_print:
    mov al, '.'
ok_print:
    mov dl, al
    mov ah, 02h
    int 21h
    inc si
    loop print_ascii

    
    ; Новая строка
    mov dx, offset newline
    mov ah, 09h
    int 21h

    dec bx
    jnz next_line

    pop ds
    ret
dump_memory endp

; -------------------------------------------------------------
; get_hex_word ? ввод 4 hex-символов, результат в AX
; -------------------------------------------------------------
get_hex_word proc
    xor bx, bx
    mov cx, 4
next_digit:
    mov ah, 01h
    int 21h

    cmp al, '0'
    jb next_digit
    cmp al, '9'
    jbe decimal
    cmp al, 'A'
    jb next_digit
    cmp al, 'F'
    jbe upper
    cmp al, 'a'
    jb next_digit
    cmp al, 'f'
    jbe lower
    jmp next_digit

decimal:
    sub al, '0'
    jmp store
upper:
    sub al, 'A'
    add al, 10
    jmp store
lower:
    sub al, 'a'
    add al, 10
store:
    shl bx, 4
    or bl, al
    loop next_digit
    mov ax, bx
    ret
get_hex_word endp

; -------------------------------------------------------------
; print_hex_byte ? вывод AL в hex
; -------------------------------------------------------------
print_hex_byte proc
    push ax
    push bx
    push dx

    mov ah, 0
    mov bl, al
    shr al, 4
    and al, 0Fh
    add al, '0'
    cmp al, '9'
    jbe skip_upper1
    add al, 7
skip_upper1:
    mov dl, al
    mov ah, 02h
    int 21h

    mov al, bl
    and al, 0Fh
    add al, '0'
    cmp al, '9'
    jbe skip_upper2
    add al, 7
skip_upper2:
    mov dl, al
    mov ah, 02h
    int 21h

    pop dx
    pop bx
    pop ax
    ret
print_hex_byte endp


; -------------------------------------------------------------
; print_hex_word ? вывод AX в hex (старший, младший байт)
; -------------------------------------------------------------
print_hex_word proc
    push ax
    mov al, ah
    call print_hex_byte
    pop ax
    call print_hex_byte
    ret
print_hex_word endp

CLRF proc near
    push dx
    mov dl, 13
    call PUTCH
    mov dl, 10
    call PUTCH
    pop dx
    ret
CLRF endp

PUTCH proc near
    push ax
    mov ah, 2h
    int 21h
    pop ax
    ret
PUTCH endp

GETCH proc
    mov AH,08h
    int 21h
    RET
GETCH endp

cseg ends
end start
