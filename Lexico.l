%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <float.h>
#include "y.tab.h"
FILE *yyin;
char *yyltext;
int cota_error(char* mensaje);
%}


%option noyywrap
%option yylineno

DIGITO 	[0-9]
LETRA 	[a-zA-ZñÑ]
ID		{LETRA}({LETRA}|{DIGITO})*
CTE_INT		{DIGITO}+
CTE_FLOAT	{DIGITO}+"."{DIGITO}*|{DIGITO}*"."{DIGITO}+
LETRAODIGITO	{LETRA}|{DIGITO}|.
CTE_STRING	\"{LETRAODIGITO}*\"
COMENTARIO	\-\/.*\/\-
ESPACIO     " "|\t

%%
 /* Inicio y fin de programa */
"START"						{ return START;}
"END"						{ return END;}

 /* Seccion de declaracion de variables y tipos de datos */
"DECVAR"					{ return DECVAR;}
"ENDDEC"					{ return ENDDEC;}
"INT"						{ yylval.str_val = strdup(yytext); return INT;}
"FLOAT"						{ yylval.str_val = strdup(yytext); return FLOAT;}
"STRING"					{ yylval.str_val = strdup(yytext); return STRING;}

 /* Bloques de control */
"WHILE"					{ return WHILE;}
"ENDWHILE"				{ return ENDWHILE;}
"IF"					{ return IF;}
"THEN"					{ return THEN;}
"ELSE"					{ return ELSE;}
"ENDIF"					{ return ENDIF;}

 /* Operadores logicos */
"&&"							{ return AND;}
"||"							{ return OR;}
"!"						{ return NOT;}

 /* Operadores */
":="						{ return ASIG;}
"="							{ return ASIGMULT;}
"+"							{ return MAS;}
"-"							{ return MENOS;}
"*"							{ return POR;}
"/"							{ return DIVIDIDO;}
"MOD"						{ return MOD;}
"DIV"						{ return DIV;}
"between"   				{ return BETWEEN;}

 /* Comparadores */
"<"							{ return MENOR;}
">"							{ return MAYOR;}
"<="						{ return MENOR_IGUAL;}
">="						{ return MAYOR_IGUAL;}
"=="						{ return IGUAL;}
"!="						{ return DISTINTO;}

 /* Otros simbolos */
"("							{ return PA;}
")"							{ return PC;}
"["							{ return CA;}
"]"							{ return CC;}
"{"							{ return LLA;}
"}"							{ return LLC;}
","							{ return COMA;}
";"							{ return PUNTO_COMA;}
":"							{ return DOS_PUNTOS;}

 /* Funciones */
"READ"						{ return READ;}
"WRITE"						{ return WRITE;}
"DECVAR"    				{ return DECVAR;}
"ENDDEC"    				{ return ENDDEC;}

 /* Identificadores y constantes */
{ID}						{ 
								if(strlen(yytext) <= 30){								
									yylval.str_val = strdup(yytext);
									return ID;
								}
								else
								cota_error("Intenta limitar tus nombres de variables a 30 caracteres.");
							}
{CTE_INT}					{
								int aux= atoi(yytext);
								if(aux<32767 && aux>-32768 ){
									yylval.int_val = aux;
									return CTE_INT;
								}
								else
								cota_error("superaste el tamanio maximo para numeros enteros.");
							}
{CTE_FLOAT}					{
								float auxf=atof(yytext);
								if(auxf<FLT_MAX && auxf>FLT_MIN){
									yylval.real_val = auxf;
									return CTE_FLOAT;
								}
								else
								cota_error("superaste el tamanio maximo para numeros reales.");
							}
{CTE_STRING}				{ 
								if(strlen(yytext)<=32){
									yylval.str_val = strdup(yytext);
									return CTE_STRING;
								}
								else
								cota_error("intenta limitar tus constantes string a 30 caracteres.");
							}
{COMENTARIO}
{ESPACIO}
\n
\t
.	{	
		char msg[] = "Token no reconocido: %s\n";
		//yyerror(msg);
		printf("Syntax Error (linea %d): %s\n", yylineno, msg);
		system ("Pause");
		exit (1);
		
	};
%%

int cota_error(char* mensaje)
{
	printf("Error lexico: %s\n", mensaje);
	system ("Pause");
	exit (1);
}

//int yyerror(char * mensaje){
//  	
//}