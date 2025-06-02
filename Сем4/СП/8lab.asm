data segment               
	 msg1	 db '�뢮� ���祭�� ॣ���஢:$'
	 msg2	 db '�뢮� ����� ����� �� ��������� �����:$'
	 el      db 2 dup ('0')
	 hx      db 4 dup (32)
	 sg      dw 0
	 adr     dw 0
	 def     db ' - $'
	 regCS   db 'CS: $'
	 regDS   db 'DS: $'
	 regSS   db 'SS: $'
	 regES   db 'ES: $'
	 regSP   db 'SP: $'
	 regSI   db 'SI: $'
	 regDI   db 'DI: $'
	 regBP   db 'BP: $'
data ends                   ; ����� ᥣ���� ������
; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
stk segment stack           ; ��砫� ᥣ���� �⥪�
    db 256 dup (0)          ; १�ࢨ஢���� 256 ���� ��� �⥪�
stk ends                    ; ����� ᥣ���� �⥪�
; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
code segment

assume CS:code,DS:data      ; ᮮ⢥��⢨� ᥣ������ ॣ���஢

begin: mov AX,data
	   mov DS,AX
	   
	   lea dx, msg1
	   mov ah, 09h
	   int 21h
	   call CLRF

; �뢮� �� �࠭ ॣ���� CS
	   mov AH,09h
	   mov DX,offset regCS
	   int 21h
	   mov AX,CS
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� DS
	   mov AH,09h
	   mov DX,offset regDS
	   int 21h
	   mov AX,DS
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� SS
	   mov AH,09h
	   mov DX,offset regSS
	   int 21h
	   mov AX,SS
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� ES
	   mov AH,09h
	   mov DX,offset regES
	   int 21h
	   mov AX,ES
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� SP
	   mov AH,09h
	   mov DX,offset regSP
	   int 21h
	   mov AX,SP
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� SI
	   mov AH,09h
	   mov DX,offset regSI
	   int 21h
	   mov AX,SI
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� DI
	   mov AH,09h
	   mov DX,offset regDI
	   int 21h
	   mov AX,DI
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h
	   CALL CLRF
; �뢮� �� �࠭ ॣ���� BP
	   mov AH,09h
	   mov DX,offset regBP
	   int 21h
	   mov AX,BP
	   CALL HEX4
	   mov AH,40h
	   mov CX,4
	   mov BX,1
	   mov DX,offset hx
	   int 21h

; ������ ����
	   call CLRF
	   lea dx, msg2
	   mov ah, 09h
	   int 21h
	   ;call CLRF
	   
	   xor BX,BX
	   mov SI,0
	   CALL inpthex         ; ��楤�� ����� ����
	   mov sg,BX            ; ��஭��� � ��६����� ᥣ����
	   mov AH,02h
	   mov DL,58
	   int 21h
	   xor BX,BX
	   mov SI,0
	   CALL inpthex
	   mov adr,BX           ; ��஭��� � ��६����� ᬥ饭�� �� ᥣ�����
	   CALL CLRF
	   CALL CLRF
	   mov AX,sg            ; �⮡� 㪠���� ES �� ������ ᥣ����
	   mov ES,AX
	   mov BX,adr           ; ��������, ��-� �� ��� ��譨�,
	   mov AX,adr           ; �� � 㦥 �� �����, ���� �諠
	   push AX

	   pop AX
	   sub AX,24            ; ��⮬� �� ��⮬ � 横�� �㤥� �ਡ������ �� 24,
	                        ; �� ��� �뢮�� � ��砫� ������ ��ப� �� ����
	   CALL dumping         ; ��楤�� �뢮�� ����� �� �����

; �����襭�� �ணࠬ�� � ����� �訡�� 0
       mov AX,4C00h
	   int 21h
; ------------------------------------------------------------------------------
HEX proc
; ��� ����祭�� ����� ������ ����
        mov CX,0
        mov BX,16
		xor DX,DX
		cmp AL,10    ; �᫨ ⠬ �� �㫨, � ���� �᫮ ����� 16
		jae dvsn     ; � ��� ��ଠ�쭮
		mov el[1],48 ; ���� ���襬 ���� � ��⮬ ��� ��ଠ�쭮
		div BX
		mov CX,1
		mov SI,1
		jmp z
dvsn:   div BX       ; ����� AX �� BX, १���� � AX, ���⮪ � DX
	    push DX      ; ᪨�㫨 ���⮪ � �⥪
	    xor DX,DX    ; ���㫨�� DX, ���� ������� �� �������
	    inc CX       ; 㢥��稫� ����稪 ������⢠ ��� � �᫥
	    test AX,AX   ; �᫨ ���� ��� ������� ��楫�,
	    jnz dvsn     ; � ����� ��� ࠧ
        mov SI,0
ot:     pop DX       ; ����﫨 ����� ���� �����筮�� �᫠
z:      cmp DL,10
		jae letter
		add DX,48    ; ����稫� �� ASCII-���
		jmp nx
letter: add DX,55    ; ����稫� ASCII-��� �㪢�
nx:     mov el[SI],DL
        inc SI
	    loop ot      ; �������� CX ࠧ (�� �������� ���)
        RET
HEX endp
; ------------------------------------------------------------------------------
HEX4 proc
; ��� ����祭�� ����� ᫮��
        push CX
		xor CX,CX
		mov CL,12
		mov SI,0
ipt:    push AX
		sar AX,CL
		and AX,0Fh
		cmp AL,0
		jne no
		mov hx[SI],48
no:     cmp AL,10
		jae lr
		add AL,48
		jmp n
lr:     add AL,55
n:      mov hx[SI],AL
        inc SI
		pop AX
		sub CL,4
	    cmp SI,4
		jb ipt
		pop CX
        RET
HEX4 endp
; ------------------------------------------------------------------------------
inpthex proc
; ���� ����
inpt:   mov AH,08h ; �㭪�� ����� ᨬ���� ��� ��� ������
        int 21h
		cmp AL,13  ; �᫨ �� ENTER,
		je inpt    ; � ��������㭣, ������� �� 4 ᨬ����, �� �����
		cmp AL,27  ; �᫨ ����� ESC,
		je fin     ; � �� �����襭��
		cmp AL,'0' ; �᫨ ��� ����񭭮�� ᨬ����...
		jb inpt    ; ����� ���� 0, � �� �����
	    cmp AL,'9' ; �᫨ ��� ����񭭮�� ᨬ����...
	    jbe ok     ; ����� ��� ࠢ�� ���� 9, � �����
	    cmp AL,'F' ; �᫨ ��� ����񭭮�� ᨬ����...
	    ja inpt    ; ����� ���� F, � �� �����
	    cmp AL,'A' ; �᫨ ��� ����񭭮�� ᨬ����...
	    jb inpt    ; ����� ���� A, � �� �����
ok:     mov AH,02h
        mov DL,AL  ; ����⠥� ����񭭮� � �������
	    int 21h
	    cmp AL,'9' ; �஢�ਬ, �� �� � ���
	    ja lttr    ; �᫨ ��� ����� ���� 9, � �㪢�
	    sub AL,'0' ; ���⥬ �� ���� 48, ����稬 ॠ�쭮 ����
	    and AX,0Fh ; ����� ��⠫�� ���� ॣ����
	    jmp addd
lttr:   sub AL,55  ; � �� �㪢� ���⠥� 55
        and AX,0Fh ; ⮦� ����૨ �� ��⠫�� ����
addd:   mov CL,4   ; 㬭����� �� 16,
        sal BX,CL  ; ᤢ����� �� 4 ��� �����,
		or BX,AX   ; �������� ����� ����
		inc SI
		cmp SI,4
		jae fin    ; �᫨ 㦥 4 �������, � ���,
		jmp inpt   ; � �᫨ ���, � �� ���������
fin:    RET
inpthex endp
; ------------------------------------------------------------------------------
dumping proc
; ����祭�� ����� ᨬ����� �� ����� � �뢮� ����� �� �࠭
        mov CX,16
dump:   add AX,24
        push AX
        push BX
        push CX
		CALL HEX4
		mov AH,40h
		mov CX,4
		mov BX,1
		mov DX,offset hx
		int 21h
		mov AH,09h
		mov DX,offset def
		int 21h
		pop CX
		pop BX
        push CX
	    mov CX,24
dmp:    xor AX,AX
        push BX
	    mov AL,ES:BX
        push AX
	    push CX
	    CALL HEX
	    mov AH,40h
	    mov BX,1
	    mov CX,2
	    mov DX,offset el
	    int 21h
	    mov el[0],48      ; �� ��直�
	    mov el[1],48      ; ���㫨� ��६�����
	    mov AH,02h
	    mov DL,32
	    int 21h
	    pop CX
	    pop AX
	    inc AX
	    pop BX
	    inc BX
	    loop dmp
	    push AX
	    CALL CLRF
	    pop AX
	    pop CX
		pop AX
	    loop dump
dumping endp
; ------------------------------------------------------------------------------
CLRF proc near
  push dx
  mov dl, 13
  call PUTCH
  mov dl, 10
  call PUTCH
  pop dx
  ret
CLRF endp
; ------------------------------------------------------------------------------
PUTCH proc near
  push ax
  mov ah, 2h
  int 21h
  pop ax
  ret
PUTCH endp
; ------------------------------------------------------------------------------
GETCH proc                  ; ��楤�� �������� ����� ᨬ����
	  mov AH,08h
	  int 21h
	  RET
GETCH endp
; ------------------------------------------------------------------------------
code ends                   ; ����� ᥣ���� ������
; -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
end begin                   ; ����� ⥪�� �ணࠬ��, ��⠭���� �窨 �室�
;===============================================================================