bison -dyv Sintactico.y
pause
flex Lexico.l
pause
bison Sintactico.y
pause
gcc.exe lex.yy.c y.tab.c -o TPFinal.exe
pause
:: TPFinal.exe prueba.txt
:: TPFinal.exe "./tests/1 write cte.txt"
:: TPFinal.exe "./tests/2 read.txt"
:: TPFinal.exe "./tests/3 if true.txt"
:: TPFinal.exe "./tests/4 if false.txt"
:: TPFinal.exe "./tests/5 while.txt"
:: TPFinal.exe "./tests/6 arithmetics.txt"
:: TPFinal.exe "./tests/6 arithmetics1.txt"
:: TPFinal.exe "./tests/6 arithmetics2.txt"
:: TPFinal.exe "./tests/6 arithmetics3.txt"
:: TPFinal.exe "./tests/6 arithmetics4.txt"
:: TPFinal.exe "./tests/7 conditions.txt"
:: TPFinal.exe "./tests/8 asignacion multiple.txt"
pause
del lex.yy.c
del y.tab.c
del Sintactico.tab.c
del y.tab.h
del TPFinal.exe