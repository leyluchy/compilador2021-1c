bison -dyv Sintactico.y
::pause
flex Lexico.l
::pause
bison Sintactico.y
::pause
gcc lex.yy.c y.tab.c assembler.c -o TPFinal.exe -std=c99 -w
pause
TPFinal.exe prueba.txt
TPFinal.exe "./tests/1 write cte.txt" "Polaca-write cte.txt"
TPFinal.exe "./tests/2 read.txt" "Polaca-read.txt"
TPFinal.exe "./tests/3 if true.txt" "Polaca-if true.txt"
TPFinal.exe "./tests/4 if false.txt" "Polaca-if false.txt"
TPFinal.exe "./tests/5 while.txt" "Polaca-while.txt"
TPFinal.exe "./tests/6 arithmetics.txt" "Polaca-arithmetics.txt"
TPFinal.exe "./tests/6 arithmetics div float.txt" "Polaca-arithmetics-div-float.txt"
TPFinal.exe "./tests/6 arithmetics div int.txt" "Polaca-arithmetics-div-int.txt"
TPFinal.exe "./tests/6 arithmetics mod.txt" "Polaca-arithmetics-mod.txt"
TPFinal.exe "./tests/7 conditions.txt" "Polaca-conditions.txt"
TPFinal.exe "./tests/8 asignacion multiple.txt" "Polaca-asignacion multiple.txt"
pause
del lex.yy.c
del y.tab.c
del Sintactico.tab.c
del y.tab.h
del TPFinal.exe
del Final.asm
del ts.txt