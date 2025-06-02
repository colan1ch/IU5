.model small       ; ������ ����� small (�⤥��� ᥣ����� ���� � ������)
.stack 100h       ; �뤥�塞 256 ���� (100h) ��� �⥪

.data
    ; ���� ��� �࠭���� ���������� ��ࠬ��� (10 ����)
    input     db 10 dup(0)
    
    ; ��ப� ��� �ଠ�஢������ �뢮��
    msg_label db ' = $'          ; �뢮����� ����� �᫮� � १���⮬
    dozen_str db ' �� + $'      ; ����� " �� + "
    end_str   db 0Dh,0Ah,'$'     ; ��ॢ�� ��ப� (CR+LF)
    
    ; ���� ��� �८�ࠧ������ �ᥫ � ��ப� (�� 5 ��� + '$')
    buffer    db 6 dup(0)

.code
main PROC
    ; ���樠������ ᥣ���� ������
    mov ax, @data
    mov ds, ax

    ; ����砥� ���� PSP (Program Segment Prefix)
    mov ah, 51h     ; �㭪�� DOS "Get PSP address"
    int 21h         ; �맮� ���뢠��� DOS
    mov es, bx      ; ES ⥯��� 㪠�뢠�� �� PSP

    ; �⠯� ࠡ��� �ணࠬ��:
    ; 1. ���뢠�� ��ࠬ��� ��������� ��ப�
    call parse_param
    
    ; 2. �८�ࠧ㥬 ��ப� � �᫮
    call atoi
    
    ; 3. ���࠭塞 �᫮ � BX
    mov bx, ax

    ; 4. �뢮��� ��室��� �᫮
    lea dx, input
    mov ah, 09h
    int 21h

    ; 5. �뢮��� ࠧ����⥫� " = "
    lea dx, msg_label
    mov ah, 09h
    int 21h

    ; 6. ����塞 ������⢮ �� (����� �� 12)
    mov ax, bx      ; ����㦠�� ��� �᫮ � AX
    xor dx, dx      ; ����塞 DX ��� �������
    mov cx, 12      ; ����⥫� = 12 (1 ���)
    div cx          ; AX = ��⭮� (���), DX = ���⮪
    
    ; 7. ���࠭塞 ���⮪ � �⥪�
    push dx

    ; 8. �뢮��� ������⢮ ��
    call print_num

    ; 9. �뢮��� ⥪�� " �� + "
    lea dx, dozen_str
    mov ah, 09h
    int 21h

    ; 10. �뢮��� ���⮪
    pop dx          ; ���⠥� ���⮪ �� �⥪�
    mov ax, dx      ; ����頥� � AX ��� �뢮��
    call print_num

    ; 11. ��ॢ���� ��ப�
    lea dx, end_str
    mov ah, 09h
    int 21h

    ; 12. �����蠥� �ணࠬ��
    mov ax, 4C00h
    int 21h
main ENDP

; =============================================
; ��楤�� �⥭�� ��ࠬ��� �� ��������� ��ப�
; =============================================
parse_param PROC
    mov si, 81h     ; ES:SI - ��砫� ��ࠬ��஢ � PSP
    lea di, input   ; DI - �㤠 �㤥� �����뢠��

    ; �ய�᪠�� ��砫�� �஡���
skip_spaces:
    mov al, es:[si]
    cmp al, 0Dh     ; �஢�ઠ �� ����� ��ப�
    je finish
    cmp al, ' '     ; �ய�᪠�� �஡���
    jne copy_char
    inc si
    jmp skip_spaces

    ; �����㥬 ᨬ���� ��ࠬ���
copy_char:
    mov al, es:[si]
    cmp al, 0Dh     ; �᫨ ����� ��ப� - ��室
    je finish
    cmp al, ' '     ; �᫨ �஡�� - ����� ��ࠬ���
    je finish
    mov [di], al    ; �����㥬 ᨬ��� � ����
    inc di
    inc si
    jmp copy_char

    ; �����蠥� ��ப� ᨬ����� '$'
finish:
    mov byte ptr [di], '$'
    ret
parse_param ENDP

; =============================================
; ��楤�� �८�ࠧ������ ��ப� � �᫮ (atoi)
; =============================================
atoi PROC
    lea si, input   ; SI - 㪠��⥫� �� ��ப�
    xor ax, ax      ; AX �㤥� ᮤ�ঠ�� १����
    xor bx, bx      ; BX - ��� �६����� ���祭��
    mov cx, 10      ; �᭮����� ��⥬� ��᫥���

atoi_loop:
    mov bl, [si]    ; ����㦠�� ⥪�騩 ᨬ���
    cmp bl, '$'     ; �஢�ઠ �� ����� ��ப�
    je atoi_done
    sub bl, '0'     ; �८�ࠧ㥬 ᨬ��� � ����
    cmp bl, 9       ; �஢��塞 �� �� ��� 0-9
    ja atoi_done

    ; ���������� १����: AX = AX * 10 + ���
    mul cx          ; �������� ⥪�騩 १���� �� 10
    add ax, bx      ; ������塞 ����� ����
    
    inc si          ; ���室�� � ᫥���饬� ᨬ����
    jmp atoi_loop   ; �����塞 横�

atoi_done:
    ret
atoi ENDP

; =============================================
; ��楤�� �뢮�� �᫠ �� �࠭
; =============================================
print_num PROC
    ; ���࠭塞 ॣ�����
    push ax
    push bx
    push cx
    push dx
    push si
    push di

    ; ����ன�� 㪠��⥫��
    mov si, offset buffer
    mov cx, 0          ; ���稪 ���
    mov bx, 10         ; �᭮����� ��⥬�

    ; ��ࠡ�⪠ ���� ����� �᫮ = 0
    cmp ax, 0
    jne print_loop
    mov byte ptr [si], '0'
    inc si
    mov byte ptr [si], '$'
    jmp print_done

    ; ���������� �᫠ �� ���� (�������� �� 10)
print_loop:
    xor dx, dx      ; ����塞 DX ��� �������
    div bx          ; ����� AX �� 10: AX = ��⭮�, DX = ���⮪
    push dx         ; ���࠭塞 ���� (���⮪) � �⥪
    inc cx          ; �����稢��� ���稪 ���
    cmp ax, 0       ; �᫨ ��⭮� = 0 - �����稢���
    jne print_loop

    ; ��������� ���� �� �⥪� � �ନ�㥬 ��ப�
    mov di, si      ; DI - 㪠��⥫� � ����
print_pop:
    pop dx          ; ���⠥� ���� �� �⥪�
    add dl, '0'     ; �८�ࠧ㥬 ���� � ᨬ���
    mov [di], dl    ; �����뢠�� ᨬ��� � ����
    inc di          ; ���室�� � ᫥���饩 ����樨
    loop print_pop  ; �����塞 ��� ��� ���

    ; �����蠥� ��ப�
    mov byte ptr [di], '$'

    ; �뢮��� ��ப�
print_done:
    lea dx, buffer
    mov ah, 09h
    int 21h

    ; ����⠭�������� ॣ�����
    pop di
    pop si
    pop dx
    pop cx
    pop bx
    pop ax
    ret
print_num ENDP

END main