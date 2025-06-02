MYCODE SEGMENT 'CODE' 
ASSUME CS:MYCODE, DS:MYCODE 
    start_char db 128             ; Символ, введённый пользователем
    tab db "0123456789ABCDEF"   ; Таблица для HEX-перевода
    ;start_char db 128             ; Символ, введённый пользователем

START: 
    ; Настройка сегментных регистров
    PUSH CS 
    POP DS 

main_loop:
    ; Очистка экрана
    CALL CLCSCR

    ; Вывод приглашения
    MOV AH, 09h
    LEA DX, prompt_msg
    INT 21h

    ; Ввод символа (без эха)
    CALL GETCH
    MOV start_char, AL          ; Сохраняем введённый символ

    ; Проверка на выход (*)
    CMP start_char, '*'
    JE exit_program

    ; Очистка экрана и вывод заголовка
    CALL CLCSCR
    MOV AH, 09h
    LEA DX, line_msg
    INT 21h

    ; Вывод 20 символов, начиная с введённого
    MOV CX, 20
    MOV AL, start_char          ; Начинаем с введённого символа
    MOV BL, AL                  ; Сохраняем в BL для HEX-вывода

print_loop:
    ; Вывод символа и его HEX-кода
    MOV DL, start_char
    CALL PUTCH
    MOV DL, '-' 
    CALL PUTCH
    CALL HEX
    CALL CLRF

    ; Переход к следующему символу
    INC start_char
    LOOP print_loop

    ; Ожидание любой клавиши для продолжения
    CALL GETCH
    JMP main_loop              ; Повтор цикла

exit_program:
    ; Выход из программы
    MOV AH, 4Ch 
    INT 21h

; --- ПРОЦЕДУРЫ ---

; Вывод символа (DL = символ)
PUTCH PROC
    MOV AH, 02
    INT 21h
    RET
PUTCH ENDP

; Ввод символа без эха (результат в AL)
GETCH PROC
    MOV AH, 08h
    INT 21h 
    RET
GETCH ENDP

; Вывод HEX-кода символа (BL = символ)
HEX PROC
	MOV BX, offset tab
; 1b
	MOV AL, start_char
	shr AL, 4
	call mXLAT
	MOV DL, AL
	CALL PUTCH
; 2b
	MOV AL, start_char
	AND AL, 0FH
	call mXLAT
	MOV DL, AL
	CALL PUTCH
; H
	MOV DL, 48H
	CALL PUTCH
	ret
HEX ENDP
;

mXLAT proc
; Сохранение BX
    PUSH BX
; Очистка AH (AX = AL)
    XOR AH, AH 
; Вычисление смещения в таблице
    ADD BX, AX
    ;MOV BX, AX
; Загрузка символа из таблицы
    MOV AL, [BX]
; Восстановление BX
    POP BX
    RET
mXLAT endp


; Перевод строки
CLRF PROC
    MOV AH, 02h
    MOV DL, 0Dh
    INT 21h
    MOV DL, 0Ah
    INT 21h
    RET
CLRF ENDP

; Очистка экрана
CLCSCR PROC    
    MOV AX, 0600h
    MOV BH, 07h
    MOV CX, 0000h
    MOV DX, 184Fh
    INT 10h
    ; Установка курсора в (0,0)
    MOV AH, 02h
    MOV BH, 00h
    MOV DX, 0000h
    INT 10h
    RET
CLCSCR ENDP


; --- ДАННЫЕ ---
prompt_msg db 'First symbol: $'
line_msg db '----------------', 0Dh, 0Ah, '$'

MYCODE ENDS 
END START