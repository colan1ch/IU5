mycode SEGMENT 'CODE'
assume cs:mycode, ds:mycode

main PROC
 push cs
 pop ds

 mov ah, 0
 mov al, 3
 int 10h

 mov ah, 8
 int 21h

 mov ah, 0
       
 add al, 1
 push ax

 mov dl, al
 call displ
 call crlf         
 pop ax

 add al, 1
 push ax

 mov dl, al
 call displ
 call crlf         
 pop ax

 add al, 1
 push ax

 mov dl, al
 call displ
 call crlf         
 pop ax
 
 

 mov ah, 4Ch
 int 21h
main ENDP

displ PROC
 mov ah, 2
 int 21h
 ret
displ ENDP

crlf PROC
 mov dl, 0Ah
 call displ
 mov dl, 0Dh
 call displ
 ret
crlf ENDP

mycode ENDS
END main