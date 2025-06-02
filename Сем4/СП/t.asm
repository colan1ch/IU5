.model small
.stack 100h

.data
    param1 DB 3 DUP(0)         ; Буфер для числа
    msg_end DB 0Dh, 0Ah, '$'   ; Перевод строки

.code
main PROC
    mov ax, @data
    mov ds, ax
    
    call read_param   ; Считываем параметр из командной строки
    call print_param  ; Выводим параметр на экран
    
    mov ax, 4C00h
    int 21h
main ENDP

read_param PROC
    push si
    push di

    mov si, 81h  ; Командная строка начинается с 81h в PSP
    lea di, param1  ; Указатель на буфер для числа

    ; Пропуск пробелов
skip_spaces:
    mov al, [si]  ; Читаем символ из DS:[SI]
    cmp al, ' '
    jne read_digit
    inc si
    jmp skip_spaces

    ; Чтение трёхзначного числа
read_digit:
    mov al, [si]  ; Читаем символ из DS:[SI]
    cmp al, 0Dh  ; Проверка на конец строки (Enter)
    je parse_end
    mov [di], al  ; Сохраняем символ в буфер
    inc si
    inc di
    jmp read_digit

parse_end:
    mov byte ptr [di], 0  ; Завершаем строку нулевым байтом
    pop di
    pop si
    ret
read_param ENDP

print_param PROC
    lea dx, param1  ; Указатель на строку param1
    mov ah, 09h     ; DOS-функция для вывода строки
    int 21h

    lea dx, msg_end ; Перевод строки
    mov ah, 09h
    int 21h
    ret
print_param ENDP

END main