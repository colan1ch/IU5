@echo off
chcp 866>nul


SET CLEAR_SCREEN=%1
SET HELP_FILE=%2
SET DATE_1 = %3
SET DATE_2 = %4
SHIFT

REM ochishaem ekran
CLS


:MAIN_MENU
REM Menyu
ECHO 1. vyvod path
ECHO 2. versiya OS
ECHO 3. Date
ECHO 4. Prosmotr kataloga
ECHO 5. Vyzov spravki
ECHO 6. Vyhod
CHOICE /c:123456 /m "viberite punkt: "

IF ERRORLEVEL 6 GOTO ITEM6
IF ERRORLEVEL 5 GOTO ITEM5
IF ERRORLEVEL 4 GOTO ITEM4
IF ERRORLEVEL 3 GOTO ITEM3
IF ERRORLEVEL 2 GOTO ITEM2
IF ERRORLEVEL 1 GOTO ITEM1

:ITEM1
REM vyvod path
ECHO vyvod peremennoj okruzheniya PATH
PATH
PAUSE
GOTO MAIN_MENU

:ITEM2
REM versiya OS
ECHO vyvod versii OS
VER
PAUSE
GOTO MAIN_MENU


:ITEM3
date
PAUSE
GOTO MAIN_MENU


:ITEM4
REM prosmotr soderzhimogo
DIR
PAUSE
GOTO MAIN_MENU

:ITEM5
REM vyzyv fayla spravki
CALL %HELP_FILE%
PAUSE
GOTO MAIN_MENU

:ITEM6
REM nuzhno li ochishchat ekran
set counterFile=launch_counter.txt
echo 0 > "%counterFile%"
IF /I "%CLEAR_SCREEN%"=="da" CLS
ECHO Zavreshenie raboty