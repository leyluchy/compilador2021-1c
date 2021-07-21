
%{
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include "globales.h"
#include "y.tab.h"
#include "funciones.c"
#include "intermedio.c"
#include "assembler.h"

FILE  *yyin;
int yylex();
void yyerror(const char *s);
//t_polaca Polaca;
//t_pila PilaIf;
//t_pila PilaWhile;
//int IDXAsignacionMultiple;
//int ContadorPolaca;

%}

%union {
      int int_val;
      float real_val;
      char* str_val;
}

 /* Inicio y fin de programa */
%token START
%token END

 /* Seccion de declaracion de variables y tipos de datos */
%token DECVAR
%token ENDDEC
%token INT
%token FLOAT
%token STRING

 /* Bloques de control */
%token WHILE
%token ENDWHILE
%token IF
%token THEN
%token ELSE
%token ENDIF

 /* Operadores logicos */
%token AND
%token OR
%token NOT

 /* Operadores */
%token ASIG
%token ASIGMULT
%token MAS
%token MENOS
%token POR
%token DIVIDIDO
%token MOD
%token DIV
%token BETWEEN 

 /* Comparadores */
%token MENOR
%token MAYOR
%token MENOR_IGUAL
%token MAYOR_IGUAL
%token IGUAL
%token DISTINTO

 /* Otros simbolos */
%token PA
%token PC
%token CA
%token CC
%token LLA
%token LLC
%token COMA
%token PUNTO_COMA
%token DOS_PUNTOS

 /* Funciones */
%token READ
%token WRITE

 /* Identificadores y constantes */
%token ID
%token CTE_INT  
%token CTE_FLOAT
%token CTE_STRING


%%

start: 
	  programa {
      // Guardar tabla simbolos
      // Guardar polaca
      generarAssembler();
      printf("COMPILACION CORRECTA\n");
    }
	  ;

programa: 

      bloque_declaracion bloque 
      |bloque
	  ;

bloque_declaracion:
      DECVAR declaraciones ENDDEC
	  ;

declaraciones:
      declaraciones declaracion
      |declaracion
	  ;

declaracion:
      lista_id DOS_PUNTOS tipo_variable
	  {
        guardar_variables_ts();
        freeArray(&array_nombres_variables);
        initArray(&array_nombres_variables);
      }
	  ;

tipo_variable:
      INT
	    {
        tipo_dato = TIPO_INT;
      }
      |FLOAT
	    {
        tipo_dato = TIPO_FLOAT;
      }
      |STRING
	    {
        tipo_dato = TIPO_STRING;
      }
	  ;

lista_id:
      lista_id COMA ID
	    {
        insertArray(&array_nombres_variables,$<str_val>3);
      }
      | ID
	    {
        insertArray(&array_nombres_variables,$<str_val>1);
      }
	  ;

bloque: 
      bloque sentencia
      |sentencia
	  ;

sentencia: 
      asignacion
	    |asignacion_mult
      |decision
      |iteracion
      |salida
      |entrada
	  ;

salida:
      WRITE CTE_INT
	    {
            char* aux = guardar_cte_int($<int_val>2);
            PonerEnPolaca(aux);

      }
      |WRITE CTE_FLOAT
	    {
			  char* aux = guardar_cte_float($<real_val>2);
            PonerEnPolaca(aux);

	    }
      |WRITE CTE_STRING
	    {
            char* nombre_cte_string = guardar_cte_string($<str_val>2);
            PonerEnPolaca(nombre_cte_string);

      }
      |WRITE ID
	    {
			if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n");
			}
	    }		  
	  ;

entrada:
      READ ID
	  ;

asignacion: 
      ID ASIG expresion
      |ID ASIG CTE_STRING
	  {
			char * nombre_cte_string = guardar_cte_string($<str_val>3);
	  }
	  ;
	  
asignacion_mult:
	  lista ASIGMULT expresion
	  ;

lista:
	  ID ASIGMULT ID
	  |lista ASIGMULT ID
	  ;

iteracion: 
      WHILE PA condicion PC LLA bloque LLC
	  ;

decision:
      IF PA condicion PC LLA bloque LLC
      | IF PA condicion PC LLA bloque LLC ELSE LLA bloque LLC
	  ;

condicion: 
      comparacion OR comparacion
      |comparacion AND comparacion
      |comparacion
      |NOT comparacion
      |between
	  ;

comparacion: 
      expresion comparador expresion
      |expresion
	  ;

between:
      BETWEEN PA ID COMA CA expresion PUNTO_COMA expresion CC PC
	  ;

comparador:
      MAYOR
      {
		    PonerEnPolaca("MAYOR");
	    }
      |MAYOR_IGUAL      
      {
		    PonerEnPolaca("MAYOR_IGUAL");
	    }
      |MENOR_IGUAL      
      {
		    PonerEnPolaca("MENOR_IGUAL");
	    }
      |MENOR      
      {
		    PonerEnPolaca("MENOR");
	    }
      |IGUAL      
      {
		    PonerEnPolaca("IGUAL");
	    }
      |DISTINTO      
      {
		    PonerEnPolaca("DISTINTO");
	    }
	  ;

		
expresion: 
      termino
      |expresion MENOS termino
	  {
		  PonerEnPolaca("MENOS");
	  }
      |expresion MAS termino
	  {
		  PonerEnPolaca("MAS");
	  }
	  ;

termino: 
      factor
      |termino POR factor
	  {
		  PonerEnPolaca("POR");
	  }
      |termino DIVIDIDO factor
	  {
		  PonerEnPolaca("DIVIDIDO");
	  }
	  |termino DIV factor
	  {
		  PonerEnPolaca("DIV");
	  }
	  |termino MOD factor
	  {
		  PonerEnPolaca("MOD");
	  }
	  ;

factor: 
      ID
      |CTE_INT 
	  {
      char* nombre_cte_int = (char*)guardar_cte_int($<int_val>1);
      PonerEnPolaca(nombre_cte_int);
	  }
	  |MENOS CTE_INT 
	  {
      char* nombre_cte_int = (char*)guardar_cte_int($<int_val>2);
      PonerEnPolaca(nombre_cte_int);

	  }
      |CTE_FLOAT
	  {
      float valor = $<real_val>1;
      char* nombre_cte_float = (char*)guardar_cte_float(valor);
      PonerEnPolaca(nombre_cte_float);

	  }
	  |MENOS CTE_FLOAT
	  {
      float valor = $<real_val>2;
      char* nombre_cte_float = (char*)guardar_cte_float(valor);
      PonerEnPolaca(nombre_cte_float);

	  }
      |PA expresion PC
	  ;

%%
int main(int argc,char *argv[])
{
  if ((yyin = fopen(argv[1], "rt")) == NULL){
	  printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
  }
  else{
	initArray(&array_nombres_variables);
    crearTabla();
    yyparse();
    guardar_ts();
    CrearPolaca();
    freeArray(&array_nombres_variables);
  }
  fclose(yyin);
  return 0;
}
void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }