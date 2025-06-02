.model small       ; Модель памяти small (отдельные сегменты кода и данных)
.stack 100h       ; Выделяем 256 байт (100h) под стек

.data
    ; Буфер для хранения введенного параметра (10 байт)
    input     db 10 dup(0)
    
    ; Строки для форматированного вывода
    msg_label db ' = $'          ; Выводится между числом и результатом
    dozen_str db ' дюж + $'      ; Текст " дюж + "
    end_str   db 0Dh,0Ah,'$'     ; Перевод строки (CR+LF)
    
    ; Буфер для преобразования чисел в строку (до 5 цифр + '$')
    buffer    db 6 dup(0)

.code
main PROC
    ; Инициализация сегмента данных
    mov ax, @data
    mov ds, ax

    ; Получаем адрес PSP (Program Segment Prefix)
    mov ah, 51h     ; Функция DOS "Get PSP address"
    int 21h         ; Вызов прерывания DOS
    mov es, bx      ; ES теперь указывает на PSP

    ; Этапы работы программы:
    ; 1. Считываем параметр командной строки
    call parse_param
    
    ; 2. Преобразуем строку в число
    call atoi
    
    ; 3. Сохраняем число в BX
    mov bx, ax

    ; 4. Выводим исходное число
    lea dx, input
    mov ah, 09h
    int 21h

    ; 5. Выводим разделитель " = "
    lea dx, msg_label
    mov ah, 09h
    int 21h

    ; 6. Вычисляем количество дюжин (делим на 12)
    mov ax, bx      ; Загружаем наше число в AX
    xor dx, dx      ; Обнуляем DX для деления
    mov cx, 12      ; Делитель = 12 (1 дюжина)
    div cx          ; AX = частное (дюжины), DX = остаток
    
    ; 7. Сохраняем остаток в стеке
    push dx

    ; 8. Выводим количество дюжин
    call print_num

    ; 9. Выводим текст " дюж + "
    lea dx, dozen_str
    mov ah, 09h
    int 21h

    ; 10. Выводим остаток
    pop dx          ; Достаем остаток из стека
    mov ax, dx      ; Помещаем в AX для вывода
    call print_num

    ; 11. Переводим строку
    lea dx, end_str
    mov ah, 09h
    int 21h

    ; 12. Завершаем программу
    mov ax, 4C00h
    int 21h
main ENDP

; =============================================
; Процедура чтения параметра из командной строки
; =============================================
parse_param PROC
    mov si, 81h     ; ES:SI - начало параметров в PSP
    lea di, input   ; DI - куда будем записывать

    ; Пропускаем начальные пробелы
skip_spaces:
    mov al, es:[si]
    cmp al, 0Dh     ; Проверка на конец строки
    je finish
    cmp al, ' '     ; Пропускаем пробелы
    jne copy_char
    inc si
    jmp skip_spaces

    ; Копируем символы параметра
copy_char:
    mov al, es:[si]
    cmp al, 0Dh     ; Если конец строки - выход
    je finish
    cmp al, ' '     ; Если пробел - конец параметра
    je finish
    mov [di], al    ; Копируем символ в буфер
    inc di
    inc si
    jmp copy_char

    ; Завершаем строку символом '$'
finish:
    mov byte ptr [di], '$'
    ret
parse_param ENDP

; =============================================
; Процедура преобразования строки в число (atoi)
; =============================================
atoi PROC
    lea si, input   ; SI - указатель на строку
    xor ax, ax      ; AX будет содержать результат
    xor bx, bx      ; BX - для временных значений
    mov cx, 10      ; Основание системы счисления

atoi_loop:
    mov bl, [si]    ; Загружаем текущий символ
    cmp bl, '$'     ; Проверка на конец строки
    je atoi_done
    sub bl, '0'     ; Преобразуем символ в цифру
    cmp bl, 9       ; Проверяем что это цифра 0-9
    ja atoi_done

    ; Накопление результата: AX = AX * 10 + цифра
    mul cx          ; Умножаем текущий результат на 10
    add ax, bx      ; Добавляем новую цифру
    
    inc si          ; Переходим к следующему символу
    jmp atoi_loop   ; Повторяем цикл

atoi_done:
    ret
atoi ENDP

; =============================================
; Процедура вывода числа на экран
; =============================================
print_num PROC
    ; Сохраняем регистры
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    ; Настройка указателей
    mov si, offset buffer
    mov cx, 0          ; Счетчик цифр
    mov bx, 10         ; Основание системы

    ; Обработка случая когда число = 0
    cmp ax, 0
    jne print_loop
    mov byte ptr [si], '0'
    inc si
    mov byte ptr [si], '$'
    jmp print_done

    ; Разделение числа на цифры (делением на 10)
print_loop:
    xor dx, dx      ; Обнуляем DX для деления
    div bx          ; Делим AX на 10: AX = частное, DX = остаток
    push dx         ; Сохраняем цифру (остаток) в стек
    inc cx          ; Увеличиваем счетчик цифр
    cmp ax, 0       ; Если частное = 0 - заканчиваем
    jne print_loop

    ; Извлекаем цифры из стека и формируем строку
    mov di, si      ; DI - указатель в буфере
print_pop:
    pop dx          ; Достаем цифру из стека
    add dl, '0'     ; Преобразуем цифру в символ
    mov [di], dl    ; Записываем символ в буфер
    inc di          ; Переходим к следующей позиции
    loop print_pop  ; Повторяем для всех цифр

    ; Завершаем строку
    mov byte ptr [di], '$'

    ; Выводим строку
print_done:
    lea dx, buffer
    mov ah, 09h
    int 21h

    ; Восстанавливаем регистры
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
print_num ENDP

END main