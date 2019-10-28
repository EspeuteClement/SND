@echo off
set CC=tool\tcc.exe

set Files=src\main.c src\snd_audio.c

rem 
%CC% -o out\SND.exe -D_CRT_SECURE_NO_WARNINGS=1 -D_STDINT_H_ -DSDL_MAIN_HANDLED -Isrc\ -Ilib\sdl\include -Llib\sdl\lib -lSDL2 %Files%