MYCODE SEGMENT 'CODE' 
ASSUME CS:MYCODE, DS:MYCODE 
	tab db "0123456789ABCDEF"
	buf	dd 0000H 
	num dw 0H
	temp db "Введите число(длинный адрес: НННН:НННН)>$Adres дампа: $Диапазон: $ - $AX=$CS=$: $"
START: 
; Загрузка сегментного регистра данных DS 
	PUSH CS 
	POP  DS 
; основной цикл
	CLD
;MLOOP:
; ввод слова
	LEA DX, temp
	CALL PUTSTR
	CALL GETDW
	CMP AL, '*'
	je close
	MOV ES, num
	MOV DL, ':'
	CALL PUTCH
	CALL GETDW
	CMP AL, '*'
	je close
	MOV SI, num
	CALL CLRF
	CALL PUTDT
; вывод слова
	MOV CX, 16
	RLOOP:
		PUSH CX
		MOV CX, 24
		MOV DX, ES
		CALL PUTADR
		MOV DL, ':'
		CALL PUTCH
		MOV DX, SI
		CALL PUTADR
		MOV DX, OFFSET temp + 78
		CALL PUTSTR
		OLOOP:
			CALL PUTDMP
			INC SI
			CMP SI, 0H
			jne EIF 
				MOV DX, ES
				INC DX
				MOV ES, DX
			EIF:
			LOOP OLOOP
		CALL CLRF
		POP CX
		LOOP RLOOP
	;jmp MLOOP
;ending:
; Ожидание завершения программы 
	;CALL CLRF
	;CALL GETCH
	;cmp AL, '*'
	;jne close
	;CALL CLCSCR
; Выход из программы 
close:
	MOV AL, 0 
	MOV AH, 4CH 
	INT 21H 
; Процедура вывода 
PUTCH PROC
	PUSH AX
	MOV AH, 02
	INT 21H
	POP AX
	RET
PUTCH ENDP
PUTSTR PROC
	PUSH AX
	MOV AH, 09
	INT 21H
	POP AX
	RET
PUTSTR ENDP
; Процедура ввода
GETCH PROC
	MOV AH, 01H
	INT 21H
	RET
GETCH ENDP
; Процедура ввода
GETDW PROC
	PUSH CX
	PUSH BX
	PUSH SI
	MOV BX, 0H
	MOV CX, 4H
	LEA DI, buf
	ILOOP:
		CLOOP:
			MOV AH, 08H
			INT 21H
			CMP AL, '*'
			je greturn
			CALL SEARCH
			jne CLOOP
		MOV [DI], AL
		MOV DL, AL
		CALL PUTCH
		INC DI
		SHL BX, 4
		ADD BX, SI
		LOOP ILOOP
greturn:
	MOV num, BX
	POP SI
	POP BX
	POP CX
	RET
GETDW ENDP
;
SEARCH PROC
	PUSH CX
	MOV CX, 16
	LEA SI, tab
	SLOOP:
		CMP [SI], AL
		je sreturn
		INC SI
		LOOP SLOOP
sreturn:
	POP CX
	RET
SEARCH ENDP
;
PUTADR PROC
	PUSH CX
	PUSH BX
	PUSH AX
	LEA BX, tab
	MOV CX, 4
	ALOOP:
		MOV AX, DX
		AND AX, 0FH
		XLAT
		PUSH AX
		SHR DX, 4
		LOOP ALOOP
	MOV CX, 4
	OALOOP:
		POP DX
		CALL PUTCH
		LOOP OALOOP
	POP AX
	POP BX
	POP CX
	RET
PUTADR ENDP
;
PUTDMP PROC
	PUSH DX
	PUSH BX
	PUSH AX
	MOV DL, [ES:SI]
	LEA BX, tab
	MOV AL, DL
	SHR AL, 4
	XLAT
	MOV DH, DL
	MOV DL, AL
	CALL PUTCH
	MOV AL, DH
	AND AL, 0FH
	XLAT
	MOV DL, AL
	CALL PUTCH
;	MOV DL, ' '
;	CALL PUTCH
	POP AX
	POP BX
	POP DX
	RET
PUTDMP ENDP
; "Введите число(длинный адрес: НННН:НННН)>$Адрес дампа: $Диапазон: $ - $AX=$CS=$:    $"
PUTDT PROC
	MOV DX, OFFSET temp + 41
	CALL PUTSTR
	MOV DX, ES
	CALL PUTADR
	MOV DL, ':'
	CALL PUTCH
	MOV DX, SI
	CALL PUTADR
	CALL CLRF
	MOV DX, OFFSET temp + 55
	CALL PUTSTR
	MOV DX, SI
	CALL PUTADR
	MOV DX, OFFSET temp + 66
	CALL PUTSTR
	MOV DX, SI
	ADD DX, 017FH
	CALL PUTADR
	CALL CLRF
	MOV DX, OFFSET temp + 70
	CALL PUTSTR
	MOV DX, AX
	CALL PUTADR
	MOV DL, 09H
	CALL PUTCH
	MOV DX, OFFSET temp + 74
	CALL PUTSTR
	MOV DX, CX
	CALL PUTADR
	CALL CLRF
	RET
PUTDT ENDP
; Процедура перевода строки
CLRF PROC
	PUSH DX
	PUSH AX
	MOV AH, 02
	MOV DL, 0AH
	INT 21H
	MOV DL, 0DH
	INT 21H
	POP AX
	POP DX
	RET
CLRF ENDP
; Процедура очистки
CLCSCR PROC
	PUSH DX
	PUSH CX
	PUSH BX
	PUSH AX
	MOV CX, 0H
	MOV DX, 184FH
	MOV BH, 7H
	MOV AX, 600H
	INT 10H
	MOV AH, 2H
	MOV DX, 0H
	MOV BH, 0H
	INT 10H
	POP AX
	POP BX
	POP CX
	POP DX
	RET
CLCSCR ENDP
MYCODE ENDS 
END START 
