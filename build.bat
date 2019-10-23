@echo off
set CC=tool\tcc.exe

rem 
%CC% -o out\SND.exe -D_CRT_SECURE_NO_WARNINGS=1 -D_STDINT_H_ -DSDL_MAIN_HANDLED -Ilib\sdl\include -Llib\sdl\lib -lSDL2 src\main.c