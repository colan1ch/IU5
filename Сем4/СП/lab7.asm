;--------------------------------------------------------------------
; �ணࠬ�� ��� ࠡ��� � ��⭠�����묨 �᫠��
; �㭪樮���:
; - ���� 4-���筮�� ��⭠����筮�� �᫠
; - �८�ࠧ������ � ��設��� �।�⠢�����
; - �뢮� � hex � �����筮� �ଠ�
; �����襭�� ࠡ���: ���� ᨬ���� '*'
;--------------------------------------------------------------------
.model small
.stack 100h

.data
    ; ��ப� ��� ����������⢨� � ���짮��⥫��
    prompt      db '������ �᫮(�ਬ��: ����)>', '$'
    error_msg   db '�訡��! ������ ���४⭮� �᫮!', 0Dh, 0Ah, '$'
    equals_sign db '=', '$'
    h_letter    db 'h ', '$'
    result      db 5 dup(' '), '$'    ; ���� ��� hex १���� (4 ᨬ���� + $)
    decimal_buf db 5 dup(' '), '$'    ; ���� ��� �����筮�� १����
    newline     db 0Dh, 0Ah, '$'      ; ������� ��ॢ��� ��ப� � ������ ���⪨
    end_msg     db '�����襭�� ����� �ᥫ!', 0Dh, 0Ah, '$'
    exit_msg    db '�ணࠬ�� �����襭�!', 0Dh, 0Ah
                db '���: ��୥� ������� ���॥���', 0Dh, 0Ah
                db '��㯯�: ��5-44�', 0Dh, 0Ah, '$'
    input_buf   db 5, 0, 5 dup(0)     ; ���� �����: ����. �����, 䠪�. �����, �����
    hex_value   dw 0                  ; ���祭�� � ��設��� �।�⠢�����

.code
;--------------------------------------------------------------------
; �᭮���� ��楤�� �ணࠬ��
;--------------------------------------------------------------------
main proc
    ; ���樠������ ᥣ���� ������
    mov ax, @data
    mov ds, ax
    
    call clear_screen              ; ���⪠ �࠭� �� ����
    
input_loop:
    ; ���� ����� � ��ࠡ�⪨ �ᥫ
    lea dx, prompt
    mov ah, 09h                    ; �㭪�� �뢮�� ��ப�
    int 21h
    
    lea dx, input_buf
    mov ah, 0Ah                    ; �㭪�� ����� ��ப�
    int 21h
    
    lea dx, newline
    mov ah, 09h
    int 21h
    
    ; �஢�ઠ �� ᨬ��� �����襭��
    mov al, input_buf+2
    cmp al, '*'
    je exit_program
    
    call process_input
    jmp input_loop

exit_program:
    ; �����襭�� �ணࠬ��
    lea dx, end_msg
    mov ah, 09h
    int 21h
    
    call clear_screen
    
    lea dx, exit_msg
    mov ah, 09h
    int 21h
    
    mov ax, 4C00h                  ; �㭪�� �����襭�� �ணࠬ��
    int 21h
main endp

;--------------------------------------------------------------------
; ��楤�� ���⪨ �࠭�
;--------------------------------------------------------------------
clear_screen proc
    mov ax, 0600h                  ; AH=06 (�ப��⪠), AL=00 (���� �࠭)
    mov bh, 07h                    ; ��ਡ��: ��� 䮭, ���� ⥪��
    xor cx, cx                     ; ���孨� ���� 㣮� (0,0)
    mov dx, 184Fh                  ; ������ �ࠢ� 㣮� (24,79)
    int 10h
    
    mov ah, 02h                    ; ��⠭���� �����
    xor bh, bh                     ; ��࠭�� 0
    xor dx, dx                     ; ������ (0,0)
    int 10h
    ret
clear_screen endp

;--------------------------------------------------------------------
; ��楤�� ��ࠡ�⪨ ���������� �᫠
;--------------------------------------------------------------------
process_input proc
    push ax bx cx dx si            ; ���࠭���� ॣ���஢
    
    ; �஢�ઠ ����� �����
    cmp byte ptr input_buf+1, 4
    jne input_error
    
    ; ���樠������ ��� �������樨
    xor ax, ax
    mov hex_value, ax
    mov cx, 4                      ; ���稪 ���
    mov si, 2                      ; �����⥫� �� ��砫� ������
    
convert_loop:
    ; �८�ࠧ������ hex � �᫮
    mov al, input_buf[si]
    call is_hex_char              ; �஢�ઠ ���४⭮�� ᨬ����
    jc input_error
    
    ; �८�ࠧ������ ASCII � ���祭��
    sub al, '0'
    cmp al, 9
    jle skip_letter
    sub al, 7                     ; ���४�� ��� A-F
skip_letter:
    cbw                          ; ����७�� AL �� AX
    push ax
    
    ; ����� �।��饣� ���祭��
    mov ax, hex_value
    shl ax, 4                    ; ��������� �� 16
    mov hex_value, ax
    
    pop ax
    add hex_value, ax           ; ���������� ����� ����
    
    inc si
    loop convert_loop
    
    ; �뢮� ���������� �᫠
    mov cx, 4
    mov si, 2
echo_loop:
    mov dl, input_buf[si]
    mov ah, 02h                  ; �뢮� ᨬ����
    int 21h
    inc si
    loop echo_loop
    
    ; �뢮� ����� ࠢ���⢠
    lea dx, equals_sign
    mov ah, 09h
    int 21h
    
    ; �८�ࠧ������ � �뢮� � hex
    call convert_to_hex
    
    lea dx, result
    mov ah, 09h
    int 21h
    
    ; �뢮� ���䨪� h
    lea dx, h_letter
    mov ah, 09h
    int 21h
    
    ; �८�ࠧ������ � �뢮� � �����筮� ����
    call convert_to_decimal
    
    lea dx, decimal_buf
    mov ah, 09h
    int 21h
    
    ; ����� ��ப�
    lea dx, newline
    mov ah, 09h
    int 21h
    jmp process_exit
    
input_error:
    ; ��ࠡ�⪠ �訡�� �����
    lea dx, error_msg
    mov ah, 09h
    int 21h
    
process_exit:
    pop si dx cx bx ax           ; ����⠭������� ॣ���஢
    ret
process_input endp

;--------------------------------------------------------------------
; �஢�ઠ, ���� �� ᨬ��� ��⭠����筮� ��ன
; �室: AL - �஢��塞� ᨬ���
; ��室: CF = 0 �᫨ ᨬ��� hex, CF = 1 �᫨ ���
;--------------------------------------------------------------------
is_hex_char proc
    cmp al, '0'
    jl not_hex
    cmp al, '9'
    jle is_hex                    ; ����� 0-9
    
    and al, 11011111b            ; �८�ࠧ������ � ���孨� ॣ����
    cmp al, 'A'
    jl not_hex
    cmp al, 'F'
    jg not_hex                   ; �㪢� A-F
    
is_hex:
    clc                         ; ������ ���� hex
    ret
not_hex:
    stc                         ; ������ �� ���� hex
    ret
is_hex_char endp

;--------------------------------------------------------------------
; �८�ࠧ������ �᫠ � hex-��ப�
; �室: hex_value - �᫮ ��� �८�ࠧ������
; ��室: result - ��ப� � hex-�।�⠢������
;--------------------------------------------------------------------
convert_to_hex proc
    push ax bx cx dx si          ; ���࠭���� ॣ���஢
    
    mov si, offset result
    mov ax, hex_value
    mov cx, 4                    ; 4 ���� ��� ��ࠡ�⪨
    
next_digit:
    rol ax, 4                    ; �뤥����� ᫥���饩 ����
    mov bl, al
    and bl, 0Fh                  ; ��᪠ ��� ������ 4 ���
    add bl, '0'                  ; �८�ࠧ������ � ASCII
    cmp bl, '9'
    jle store_digit
    add bl, 7                    ; ���४�� ��� A-F
store_digit:
    mov [si], bl
    inc si
    loop next_digit
    
    mov byte ptr [si], '$'       ; �������騩 ᨬ���
    
    pop si dx cx bx ax          ; ����⠭������� ॣ���஢
    ret
convert_to_hex endp

;--------------------------------------------------------------------
; �८�ࠧ������ �᫠ � �������� ��ப�
; �室: hex_value - �᫮ ��� �८�ࠧ������
; ��室: decimal_buf - ��ப� � ������� �।�⠢������
;--------------------------------------------------------------------
convert_to_decimal proc
    push ax bx cx dx si          ; ���࠭���� ॣ���஢
    
    lea si, decimal_buf
    mov ax, hex_value
    xor cx, cx                   ; ���稪 ���
    mov bx, 10                   ; ����⥫�
    
decimal_loop:
    xor dx, dx
    div bx                       ; ������� �� 10
    push dx                      ; ���࠭���� ���⪠ (����)
    inc cx
    test ax, ax                  ; �஢�ઠ �� ����砭�� �᫠
    jnz decimal_loop
    
store_digits:
    pop dx                       ; �����祭�� ��� � ���⭮� ���浪�
    add dl, '0'                  ; �८�ࠧ������ � ASCII
    mov [si], dl
    inc si
    loop store_digits
    
    mov byte ptr [si], '$'       ; �������騩 ᨬ���
    
    pop si dx cx bx ax          ; ����⠭������� ॣ���஢
    ret
convert_to_decimal endp

end main