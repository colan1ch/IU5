data segment               
    msg1    db '�뢮� ���祭�� ॣ���஢:$'
    msg2    db '�뢮� ����� ����� �� ��������� �����:$'
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
    chars   db 10 dup (' ')  ; ���� ��� ᨬ�����
    space   db ' $'
data ends

stk segment stack
    db 256 dup (0)
stk ends

code segment
assume CS:code,DS:data

begin: 
    mov AX,data
    mov DS,AX
    
    jmp skip

    lea dx, msg1
    mov ah, 09h
    int 21h
    call CLRF
; �뢮� ॣ���஢ (��⠥��� ��� ���������)
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
skip:
; ���� ����
    call CLRF
    lea dx, msg2
    mov ah, 09h
    int 21h
       
    xor BX,BX
    mov SI,0
    CALL inpthex
    mov sg,BX
    mov AH,02h
    mov DL,58
    int 21h
    xor BX,BX
    mov SI,0
    CALL inpthex
    mov adr,BX
    CALL CLRF
    CALL CLRF
    mov AX,sg
    mov ES,AX
    mov BX,adr
    mov AX,adr
    push AX

    pop AX
    sub AX,8            ; ���४�஢�� ��� �뢮�� ����
    CALL dumping

    mov AX,4C00h
    int 21h

; ��楤���
HEX proc
    mov CX,0
    mov BX,16
    xor DX,DX
    cmp AL,10
    jae dvsn
    mov el[1],48
    div BX
    mov CX,1
    mov SI,1
    jmp z
dvsn:   
    div BX
    push DX
    xor DX,DX
    inc CX
    test AX,AX
    jnz dvsn
    mov SI,0
ot: 
    pop DX
z:  
    cmp DL,10
    jae letter
    add DX,48
    jmp nx
letter: 
    add DX,55
nx: 
    mov el[SI],DL
    inc SI
    loop ot
    RET
HEX endp

HEX4 proc
    push CX
    xor CX,CX
    mov CL,12
    mov SI,0
ipt:
    push AX
    shr AX,CL
    and AX,0Fh
    cmp AL,0
    jne no
    mov hx[SI],48
no:
    cmp AL,10
    jae lr
    add AL,48
    jmp n
lr:
    add AL,55
n:
    mov hx[SI],AL
    inc SI
    pop AX
    sub CL,4
    cmp SI,4
    jb ipt
    pop CX
    RET
HEX4 endp

inpthex proc
inpt:
    mov AH,08h 
    int 21h
    cmp AL,13  
    je inpt    
    cmp AL,27  
    je fin     
    cmp AL,'0' 
    jb inpt    
    cmp AL,'9' 
    jbe ok     
    cmp AL,'F' 
    ja inpt    
    cmp AL,'A' 
    jb inpt    
ok:
    mov AH,02h
    mov DL,AL  
    int 21h
    cmp AL,'9' 
    ja lttr    
    sub AL,'0' 
    and AX,0Fh 
    jmp addd
lttr:
    sub AL,55  
    and AX,0Fh 
addd:
    mov CL,4   
    shl BX,CL  
    or BX,AX   
    inc SI
    cmp SI,4
    jae fin    
    jmp inpt   
fin:
    RET
inpthex endp

;==============================================
; �᭮���� ��楤�� ����� �����
;==============================================
dumping proc
    push bp
    mov bp, sp
    push bx
    push cx
    push si
    push di
    
    mov cx, 8          ; �ᥣ� 8 ��ப �����
    mov bx, adr        ; ��砫쭮� ᬥ饭��
    sub bx, 8          ; ���४�஢�� ��� ��ࢮ� ��ப�
    mov di, sg         ; ���࠭塞 ᥣ���� � DI

dump_main_loop:
    ; ��ࠡ�⪠ ����� ��ப� �����
    call dump_line
    
    ; �஢�ઠ �����襭��
    dec cx
    jnz dump_main_loop ; ��� - �த������
    
dump_exit:
    pop di
    pop si
    pop cx
    pop bx
    pop bp
    ret
dumping endp

;==============================================
; �뢮� ����� ��ப� ����� (8 ����)
;==============================================
dump_line proc
    add bx, 8          ; ������騥 8 ����
    push bx            ; ���࠭塞 ⥪�騩 ����
    push cx            ; ���࠭塞 ���稪 ��ப
    
    ; �뢮� ���� (ᥣ����:ᬥ饭��)
    call print_address
    
    ; �뢮� 8 ���� � HEX � ASCII
    call print_hex_bytes
    call print_ascii_chars
    
    call CLRF          ; ����� ��ப�
    
    pop cx             ; ����⠭�������� ���稪
    pop bx             ; ����⠭�������� ����
    ret
dump_line endp

;==============================================
; �뢮� ���� � �ଠ� XXXX:YYYY
;==============================================
print_address proc
    ; �뢮� ᥣ����
    mov ax, di
    call HEX4
    mov ah, 40h
    mov cx, 4
    mov bx, 1
    mov dx, offset hx
    int 21h
    
    ; �뢮� �������
    mov ah, 02h
    mov dl, ':'
    int 21h
    
    ; �뢮� ᬥ饭��
    mov ax, [bp-8]    ; ��६ ��࠭���� BX
    call HEX4
    mov ah, 40h
    mov cx, 4
    mov bx, 1
    mov dx, offset hx
    int 21h
    
    ; �஡�� ��᫥ ����
    mov ah, 02h
    mov dl, ' '
    int 21h
    ret
print_address endp

;==============================================
; �뢮� 8 ���� � HEX �ଠ�
;==============================================
print_hex_bytes proc
    mov si, 0          ; ���稪 ���� (0-7)
    mov cx, 8          ; 8 ���� �� ��ப�

hex_byte_loop:
    mov al, es:[bx+si] ; ��⠥� ����
    mov chars[si], al  ; ���࠭塞 ��� ASCII
    
    ; �஢�ઠ �� ����� ᨬ���
    cmp al, 32
    jb non_printable
    cmp al, 126
    ja non_printable
    jmp short print_hex
    
non_printable:
    mov chars[si], '.' ; ������ ��������

print_hex:
    call HEX           ; �뢮� ���� � HEX
    mov ah, 40h
    mov bx, 1
    mov cx, 2
    mov dx, offset el
    int 21h
    
    ; �஡�� ����� ���⠬�
    mov ah, 02h
    mov dl, ' '
    int 21h
    
    inc si            ; ������騩 ����
    
    ; �஢�ઠ �����襭��
    cmp si, 8
    jb hex_byte_loop  ; �த������ ���� SI < 8
    
    ret
print_hex_bytes endp

;==============================================
; �뢮� ASCII ᨬ�����
;==============================================
print_ascii_chars proc
    ; �஡�� ��। ᨬ������
    mov ah, 02h
    mov dl, ' '
    int 21h
    
    ; �뢮� 8 ᨬ�����
    mov ah, 40h
    mov bx, 1
    mov cx, 8
    mov dx, offset chars
    int 21h
    ret
print_ascii_chars endp

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

code ends
end begin

