REM ECHO
REM ���� ।���஢���� ��室���� �����
REM  Asm_ed.exe %1.asm
REM ���� �������樨
tasm /l /zi /c %1.asm
REM ���� ।���஢���� �痢�
tlink /m /v /l %1.obj
PAUSE
REM ���� �⫠���
 td %1.exe
REM EXIT