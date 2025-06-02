cseg segment
assume cs:cseg,ds:cseg
org 100h

start:
    jmp begin

; Данные программы
hex_digits db '0123456789ABCDEF'    ; таблица для перевода в hex
prompt db 'Введите адрес (сегмент:смещение)>$'
prefix db 13,10,'ds:$'
equals db ' = $'
new_line db 13,10,'$'

buffer dw 0   ; буфер для ввода
segment_addr dw 0  ; сегмент введенного адреса
offset_addr dw 0   ; смещение введенного адреса

begin:
    ; Установка сегментных регистров
    push cs
    pop ds

    ; Вывод приглашения для ввода
    mov dx, offset prompt
    mov ah, 09h
    int 21h

    ; Ввод сегмента
    call get_hex_word
    mov segment_addr, ax
    
    ; Вывод двоеточия
    mov dl, ':'
    mov ah, 02h
    int 21h
    
    ; Ввод смещения
    call get_hex_word
    mov offset_addr, ax

    ; Настройка для чтения памяти
    mov es, segment_addr
    mov si, offset_addr
    
    ; Вывод пяти строк по 8 байт
    mov cx, 5      ; количество строк
    
line_loop:
    push cx
    
    ; Вывод префикса ds:
    mov dx, offset prefix
    mov ah, 09h
    int 21h
    
    ; Вывод адреса в hex
    mov ax, si
    call print_hex_word
    
    ; Вывод 8 байт памяти
    mov cx, 8
    push si      ; сохраняем указатель для ASCII части
dump_loop:
    ; Вывод пробела
    mov dl, ' '
    mov ah, 02h
    int 21h
    
    ; Вывод байта в hex
    mov al, byte ptr es:[si]
    call print_hex_byte
    
    inc si
    loop dump_loop
    
    ; Вывод разделителя =
    mov dx, offset equals
    mov ah, 09h
    int 21h
    
    ; Восстановление указателя для ASCII части
    pop si
    mov cx, 8      ; 8 символов в строке
ascii_loop:
    ; Вывод символа
    mov al, byte ptr es:[si]
    cmp al, 32    ; Проверка на печатаемый символ
    jb non_print
    cmp al, 126
    ja non_print
    jmp print_char
non_print:
    mov al, '.'   ; Замена непечатаемых символов точкой
print_char:
    mov dl, al
    mov ah, 02h
    int 21h
    
    inc si
    loop ascii_loop
    
    ; Переход на новую строку
    mov dx, offset new_line
    mov ah, 09h
    int 21h
    
    pop cx          ; восстанавливаем счетчик строк
    dec cx
    jnz line_loop   ; продолжаем, если не все строки выведены

    ; Выход в DOS
    mov ax, 4c00h
    int 21h

; Процедура ввода шестнадцатеричного слова
get_hex_word proc
    xor bx, bx    ; Очистка результата
    mov cx, 4     ; Счетчик цифр (4 для слова)
input_loop:
    mov ah, 01h   ; Ввод символа
    int 21h
    
    ; Преобразование символа в число
    cmp al, '0'
    jb error_input
    cmp al, '9'
    jbe decimal
    
    cmp al, 'A'
    jb error_input
    cmp al, 'F'
    jbe hex_upper
    
    cmp al, 'a'
    jb error_input
    cmp al, 'f'
    jbe hex_lower
    
error_input:
    jmp input_loop
    
decimal:
    sub al, '0'
    jmp convert
    
hex_upper:
    sub al, 'A'
    add al, 10
    jmp convert
    
hex_lower:
    sub al, 'a'
    add al, 10
    
convert:
    shl bx, 4     ; Сдвиг влево на 4 бита
    or bl, al     ; Добавление новой цифры
    
    loop input_loop
    
    mov ax, bx    ; Возврат результата в AX
    ret
get_hex_word endp

; Процедура печати байта в hex
print_hex_byte proc
    push ax
    push bx
    push dx
    
    mov bl, al
    shr al, 4     ; Старшая тетрада
    and bl, 0fh   ; Младшая тетрада
    
    mov bx, offset hex_digits
    xlat          ; Преобразование старшей тетрады
    mov dl, al
    mov ah, 02h
    int 21h
    
    mov al, bl
    xlat          ; Преобразование младшей тетрады
    mov dl, al
    mov ah, 02h
    int 21h
    
    pop dx
    pop bx
    pop ax
    ret
print_hex_byte endp

; Процедура печати слова в hex
print_hex_word proc
    push ax
    
    ; Печать старшего байта
    mov al, ah
    call print_hex_byte
    
    ; Печать младшего байта
    pop ax
    call print_hex_byte
    
    ret
print_hex_word endp

cseg ends
end start