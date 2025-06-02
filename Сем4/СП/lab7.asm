;--------------------------------------------------------------------
; Программа для работы с шестнадцатеричными числами
; Функционал:
; - Ввод 4-значного шестнадцатеричного числа
; - Преобразование в машинное представление
; - Вывод в hex и десятичном формате
; Завершение работы: ввод символа '*'
;--------------------------------------------------------------------
.model small
.stack 100h

.data
    ; Строки для взаимодействия с пользователем
    prompt      db 'Введите число(пример: НННН)>', '$'
    error_msg   db 'Ошибка! Введите корректное число!', 0Dh, 0Ah, '$'
    equals_sign db '=', '$'
    h_letter    db 'h ', '$'
    result      db 5 dup(' '), '$'    ; Буфер для hex результата (4 символа + $)
    decimal_buf db 5 dup(' '), '$'    ; Буфер для десятичного результата
    newline     db 0Dh, 0Ah, '$'      ; Символы перевода строки и возврата каретки
    end_msg     db 'Завершение ввода чисел!', 0Dh, 0Ah, '$'
    exit_msg    db 'Программа завершена!', 0Dh, 0Ah
                db 'ФИО: Чернев Николай Андреевич', 0Dh, 0Ah
                db 'Группа: ИУ5-44Б', 0Dh, 0Ah, '$'
    input_buf   db 5, 0, 5 dup(0)     ; Буфер ввода: макс. длина, факт. длина, данные
    hex_value   dw 0                  ; Значение в машинном представлении

.code
;--------------------------------------------------------------------
; Основная процедура программы
;--------------------------------------------------------------------
main proc
    ; Инициализация сегмента данных
    mov ax, @data
    mov ds, ax
    
    call clear_screen              ; Очистка экрана при старте
    
input_loop:
    ; Цикл ввода и обработки чисел
    lea dx, prompt
    mov ah, 09h                    ; Функция вывода строки
    int 21h
    
    lea dx, input_buf
    mov ah, 0Ah                    ; Функция ввода строки
    int 21h
    
    lea dx, newline
    mov ah, 09h
    int 21h
    
    ; Проверка на символ завершения
    mov al, input_buf+2
    cmp al, '*'
    je exit_program
    
    call process_input
    jmp input_loop

exit_program:
    ; Завершение программы
    lea dx, end_msg
    mov ah, 09h
    int 21h
    
    call clear_screen
    
    lea dx, exit_msg
    mov ah, 09h
    int 21h
    
    mov ax, 4C00h                  ; Функция завершения программы
    int 21h
main endp

;--------------------------------------------------------------------
; Процедура очистки экрана
;--------------------------------------------------------------------
clear_screen proc
    mov ax, 0600h                  ; AH=06 (прокрутка), AL=00 (весь экран)
    mov bh, 07h                    ; Атрибут: черный фон, белый текст
    xor cx, cx                     ; Верхний левый угол (0,0)
    mov dx, 184Fh                  ; Нижний правый угол (24,79)
    int 10h
    
    mov ah, 02h                    ; Установка курсора
    xor bh, bh                     ; Страница 0
    xor dx, dx                     ; Позиция (0,0)
    int 10h
    ret
clear_screen endp

;--------------------------------------------------------------------
; Процедура обработки введенного числа
;--------------------------------------------------------------------
process_input proc
    push ax bx cx dx si            ; Сохранение регистров
    
    ; Проверка длины ввода
    cmp byte ptr input_buf+1, 4
    jne input_error
    
    ; Инициализация для конвертации
    xor ax, ax
    mov hex_value, ax
    mov cx, 4                      ; Счетчик цифр
    mov si, 2                      ; Указатель на начало данных
    
convert_loop:
    ; Преобразование hex в число
    mov al, input_buf[si]
    call is_hex_char              ; Проверка корректности символа
    jc input_error
    
    ; Преобразование ASCII в значение
    sub al, '0'
    cmp al, 9
    jle skip_letter
    sub al, 7                     ; Коррекция для A-F
skip_letter:
    cbw                          ; Расширение AL до AX
    push ax
    
    ; Сдвиг предыдущего значения
    mov ax, hex_value
    shl ax, 4                    ; Умножение на 16
    mov hex_value, ax
    
    pop ax
    add hex_value, ax           ; Добавление новой цифры
    
    inc si
    loop convert_loop
    
    ; Вывод введенного числа
    mov cx, 4
    mov si, 2
echo_loop:
    mov dl, input_buf[si]
    mov ah, 02h                  ; Вывод символа
    int 21h
    inc si
    loop echo_loop
    
    ; Вывод знака равенства
    lea dx, equals_sign
    mov ah, 09h
    int 21h
    
    ; Преобразование и вывод в hex
    call convert_to_hex
    
    lea dx, result
    mov ah, 09h
    int 21h
    
    ; Вывод суффикса h
    lea dx, h_letter
    mov ah, 09h
    int 21h
    
    ; Преобразование и вывод в десятичном виде
    call convert_to_decimal
    
    lea dx, decimal_buf
    mov ah, 09h
    int 21h
    
    ; Новая строка
    lea dx, newline
    mov ah, 09h
    int 21h
    jmp process_exit
    
input_error:
    ; Обработка ошибки ввода
    lea dx, error_msg
    mov ah, 09h
    int 21h
    
process_exit:
    pop si dx cx bx ax           ; Восстановление регистров
    ret
process_input endp

;--------------------------------------------------------------------
; Проверка, является ли символ шестнадцатеричной цифрой
; Вход: AL - проверяемый символ
; Выход: CF = 0 если символ hex, CF = 1 если нет
;--------------------------------------------------------------------
is_hex_char proc
    cmp al, '0'
    jl not_hex
    cmp al, '9'
    jle is_hex                    ; Цифры 0-9
    
    and al, 11011111b            ; Преобразование в верхний регистр
    cmp al, 'A'
    jl not_hex
    cmp al, 'F'
    jg not_hex                   ; Буквы A-F
    
is_hex:
    clc                         ; Символ является hex
    ret
not_hex:
    stc                         ; Символ не является hex
    ret
is_hex_char endp

;--------------------------------------------------------------------
; Преобразование числа в hex-строку
; Вход: hex_value - число для преобразования
; Выход: result - строка с hex-представлением
;--------------------------------------------------------------------
convert_to_hex proc
    push ax bx cx dx si          ; Сохранение регистров
    
    mov si, offset result
    mov ax, hex_value
    mov cx, 4                    ; 4 цифры для обработки
    
next_digit:
    rol ax, 4                    ; Выделение следующей цифры
    mov bl, al
    and bl, 0Fh                  ; Маска для младших 4 бит
    add bl, '0'                  ; Преобразование в ASCII
    cmp bl, '9'
    jle store_digit
    add bl, 7                    ; Коррекция для A-F
store_digit:
    mov [si], bl
    inc si
    loop next_digit
    
    mov byte ptr [si], '$'       ; Завершающий символ
    
    pop si dx cx bx ax          ; Восстановление регистров
    ret
convert_to_hex endp

;--------------------------------------------------------------------
; Преобразование числа в десятичную строку
; Вход: hex_value - число для преобразования
; Выход: decimal_buf - строка с десятичным представлением
;--------------------------------------------------------------------
convert_to_decimal proc
    push ax bx cx dx si          ; Сохранение регистров
    
    lea si, decimal_buf
    mov ax, hex_value
    xor cx, cx                   ; Счетчик цифр
    mov bx, 10                   ; Делитель
    
decimal_loop:
    xor dx, dx
    div bx                       ; Деление на 10
    push dx                      ; Сохранение остатка (цифры)
    inc cx
    test ax, ax                  ; Проверка на окончание числа
    jnz decimal_loop
    
store_digits:
    pop dx                       ; Извлечение цифр в обратном порядке
    add dl, '0'                  ; Преобразование в ASCII
    mov [si], dl
    inc si
    loop store_digits
    
    mov byte ptr [si], '$'       ; Завершающий символ
    
    pop si dx cx bx ax          ; Восстановление регистров
    ret
convert_to_decimal endp

end main