include macros2.asm
include number.asm

.MODEL SMALL
.386
.STACK 200h

.DATA
NEW_LINE DB 0AH,0DH,'$'
CWprevio DW ?
i1 dd ?
i2 dd ?
i3 dd ?
_cte_string_0 db ""Test asig mult cte: da dos 1"", '$'
_1 dd 1
_cte_string_1 db ""Test asig mult var: da dos 2"", '$'
_2 dd 2

.CODE

MOV AX, @DATA
MOV DS, AX
FINIT


MOV AH, 1
INT 21h
MOV AX, 4C00h
INT 21h

END
