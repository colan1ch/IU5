.model small
.stack 100h

.data
    SURNAME_STR DB 'Чернев', 0
    
    msg_param1_ok DB 'Первый параметр = Чернев', 0Dh, 0Ah, '$'
    msg_param1_err DB 'Первый параметр неверен', 0Dh, 0Ah, '$'
    msg_param2_yes DB 'Второй параметр есть', 0Dh, 0Ah, '$'
    msg_param2_no DB 'Второй параметр отсутствует', 0Dh, 0Ah, '$'

    raw_params DB 128 DUP(0)
    param1 DB 30 DUP(0)
    param2 DB 30 DUP(0)

.code
main PROC
    mov ax, @data
    mov ds, ax

    call copy_raw_params
    call parse_params
    call check_param1
    call check_param2

    mov ax, 4C00h
    int 21h
main ENDP

copy_raw_params PROC
    push cx
    push si
    push di

    mov ax, es          
    mov si, 81h         
    lea di, raw_params

    mov cl, es:[80h]    
    cmp cl, 0
    je no_params
    xor ch, ch


copy_loop:

    mov al, es:[si]
    mov [di], al
    inc si
    inc di
    loop copy_loop

no_params:
    mov byte ptr [di], 0 
    pop di
    pop si
    pop cx
    ret
copy_raw_params ENDP

parse_params PROC
    push si
    push di

    lea si, raw_params

skip_spaces:
    cmp byte ptr [si], 0
    je parse_end
    cmp byte ptr [si], ' '
    jne parse_param1
    inc si
    jmp skip_spaces

parse_param1:
    lea di, param1

copy_param1:
    cmp byte ptr [si], 0
    je param1_end
    cmp byte ptr [si], ' '
    je param1_end
    mov al, [si]
    mov [di], al
    inc si
    inc di
    jmp copy_param1

param1_end:
    mov byte ptr [di], 0

skip_spaces2:
    cmp byte ptr [si], 0
    je parse_end
    cmp byte ptr [si], ' '
    jne parse_param2
    inc si
    jmp skip_spaces2

parse_param2:
    lea di, param2

copy_param2:
    cmp byte ptr [si], 0
    je param2_end
    cmp byte ptr [si], ' '
    je param2_end
    mov al, [si]
    mov [di], al
    inc si
    inc di
    jmp copy_param2

param2_end:
    mov byte ptr [di], 0

parse_end:
    pop di
    pop si
    ret
parse_params ENDP

check_param1 PROC
    push si
    push di

    mov si, offset param1
    mov di, offset SURNAME_STR

string_compare:
    mov al, [si]
    mov bl, [di]
    cmp al, 0
    jne continue_compare
    cmp bl, 0
    jne strings_different
    jmp strings_equal

continue_compare:
    cmp al, bl
    jne strings_different
    inc si
    inc di
    jmp string_compare

strings_equal:
    lea dx, msg_param1_ok
    call print_string
    jmp check_param1_end

strings_different:
    lea dx, msg_param1_err
    call print_string

check_param1_end:
    pop di
    pop si
    ret
check_param1 ENDP

check_param2 PROC
    cmp byte ptr [param2], 0
    je no_second_param

    lea dx, msg_param2_yes
    jmp print_and_exit

no_second_param:
    lea dx, msg_param2_no

print_and_exit:
    call print_string
    ret
check_param2 ENDP

print_string PROC
    push ax
    mov ah, 09h
    int 21h
    pop ax
    ret
print_string ENDP

END main
