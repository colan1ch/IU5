cseg segment
assume cs:cseg,ds:cseg
org 100h

start:
    jmp begin

; ����� �ணࠬ��
hex_digits db '0123456789ABCDEF'    ; ⠡��� ��� ��ॢ��� � hex
prompt db '������ ���� (ᥣ����:ᬥ饭��)>$'
prefix db 13,10,'ds:$'
equals db ' = $'
new_line db 13,10,'$'

buffer dw 0   ; ���� ��� �����
segment_addr dw 0  ; ᥣ���� ���������� ����
offset_addr dw 0   ; ᬥ饭�� ���������� ����

begin:
    ; ��⠭���� ᥣ������ ॣ���஢
    push cs
    pop ds

    ; �뢮� �ਣ��襭�� ��� �����
    mov dx, offset prompt
    mov ah, 09h
    int 21h

    ; ���� ᥣ����
    call get_hex_word
    mov segment_addr, ax
    
    ; �뢮� �������
    mov dl, ':'
    mov ah, 02h
    int 21h
    
    ; ���� ᬥ饭��
    call get_hex_word
    mov offset_addr, ax

    ; ����ன�� ��� �⥭�� �����
    mov es, segment_addr
    mov si, offset_addr
    
    ; �뢮� ��� ��ப �� 8 ����
    mov cx, 5      ; ������⢮ ��ப
    
line_loop:
    push cx
    
    ; �뢮� ��䨪� ds:
    mov dx, offset prefix
    mov ah, 09h
    int 21h
    
    ; �뢮� ���� � hex
    mov ax, si
    call print_hex_word
    
    ; �뢮� 8 ���� �����
    mov cx, 8
    push si      ; ��࠭塞 㪠��⥫� ��� ASCII ���
dump_loop:
    ; �뢮� �஡���
    mov dl, ' '
    mov ah, 02h
    int 21h
    
    ; �뢮� ���� � hex
    mov al, byte ptr es:[si]
    call print_hex_byte
    
    inc si
    loop dump_loop
    
    ; �뢮� ࠧ����⥫� =
    mov dx, offset equals
    mov ah, 09h
    int 21h
    
    ; ����⠭������� 㪠��⥫� ��� ASCII ���
    pop si
    mov cx, 8      ; 8 ᨬ����� � ��ப�
ascii_loop:
    ; �뢮� ᨬ����
    mov al, byte ptr es:[si]
    cmp al, 32    ; �஢�ઠ �� ���⠥�� ᨬ���
    jb non_print
    cmp al, 126
    ja non_print
    jmp print_char
non_print:
    mov al, '.'   ; ������ �����⠥��� ᨬ����� �窮�
print_char:
    mov dl, al
    mov ah, 02h
    int 21h
    
    inc si
    loop ascii_loop
    
    ; ���室 �� ����� ��ப�
    mov dx, offset new_line
    mov ah, 09h
    int 21h
    
    pop cx          ; ����⠭�������� ���稪 ��ப
    dec cx
    jnz line_loop   ; �த������, �᫨ �� �� ��ப� �뢥����

    ; ��室 � DOS
    mov ax, 4c00h
    int 21h

; ��楤�� ����� ��⭠����筮�� ᫮��
get_hex_word proc
    xor bx, bx    ; ���⪠ १����
    mov cx, 4     ; ���稪 ��� (4 ��� ᫮��)
input_loop:
    mov ah, 01h   ; ���� ᨬ����
    int 21h
    
    ; �८�ࠧ������ ᨬ���� � �᫮
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
    shl bx, 4     ; ����� ����� �� 4 ���
    or bl, al     ; ���������� ����� ����
    
    loop input_loop
    
    mov ax, bx    ; ������ १���� � AX
    ret
get_hex_word endp

; ��楤�� ���� ���� � hex
print_hex_byte proc
    push ax
    push bx
    push dx
    
    mov bl, al
    shr al, 4     ; ����� ��ࠤ�
    and bl, 0fh   ; ������ ��ࠤ�
    
    mov bx, offset hex_digits
    xlat          ; �८�ࠧ������ ���襩 ��ࠤ�
    mov dl, al
    mov ah, 02h
    int 21h
    
    mov al, bl
    xlat          ; �८�ࠧ������ ����襩 ��ࠤ�
    mov dl, al
    mov ah, 02h
    int 21h
    
    pop dx
    pop bx
    pop ax
    ret
print_hex_byte endp

; ��楤�� ���� ᫮�� � hex
print_hex_word proc
    push ax
    
    ; ����� ���襣� ����
    mov al, ah
    call print_hex_byte
    
    ; ����� ����襣� ����
    pop ax
    call print_hex_byte
    
    ret
print_hex_word endp

cseg ends
end start