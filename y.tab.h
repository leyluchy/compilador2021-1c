
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     START = 258,
     END = 259,
     DECVAR = 260,
     ENDDEC = 261,
     INT = 262,
     FLOAT = 263,
     STRING = 264,
     WHILE = 265,
     ENDWHILE = 266,
     IF = 267,
     THEN = 268,
     ELSE = 269,
     ENDIF = 270,
     AND = 271,
     OR = 272,
     NOT = 273,
     ASIG = 274,
     ASIGMULT = 275,
     MAS = 276,
     MENOS = 277,
     POR = 278,
     DIVIDIDO = 279,
     MOD = 280,
     DIV = 281,
     BETWEEN = 282,
     MENOR = 283,
     MAYOR = 284,
     MENOR_IGUAL = 285,
     MAYOR_IGUAL = 286,
     IGUAL = 287,
     DISTINTO = 288,
     PA = 289,
     PC = 290,
     CA = 291,
     CC = 292,
     LLA = 293,
     LLC = 294,
     COMA = 295,
     PUNTO_COMA = 296,
     DOS_PUNTOS = 297,
     READ = 298,
     WRITE = 299,
     ID = 300,
     CTE_INT = 301,
     CTE_FLOAT = 302,
     CTE_STRING = 303
   };
#endif
/* Tokens.  */
#define START 258
#define END 259
#define DECVAR 260
#define ENDDEC 261
#define INT 262
#define FLOAT 263
#define STRING 264
#define WHILE 265
#define ENDWHILE 266
#define IF 267
#define THEN 268
#define ELSE 269
#define ENDIF 270
#define AND 271
#define OR 272
#define NOT 273
#define ASIG 274
#define ASIGMULT 275
#define MAS 276
#define MENOS 277
#define POR 278
#define DIVIDIDO 279
#define MOD 280
#define DIV 281
#define BETWEEN 282
#define MENOR 283
#define MAYOR 284
#define MENOR_IGUAL 285
#define MAYOR_IGUAL 286
#define IGUAL 287
#define DISTINTO 288
#define PA 289
#define PC 290
#define CA 291
#define CC 292
#define LLA 293
#define LLC 294
#define COMA 295
#define PUNTO_COMA 296
#define DOS_PUNTOS 297
#define READ 298
#define WRITE 299
#define ID 300
#define CTE_INT 301
#define CTE_FLOAT 302
#define CTE_STRING 303




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 26 "Sintactico.y"

      int int_val;
      float real_val;
      char* str_val;



/* Line 1676 of yacc.c  */
#line 156 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


