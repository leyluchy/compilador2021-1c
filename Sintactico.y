
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
      char* auxVar;
      
      pila* pilaComp;
      pila* pilaAsigMult;
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
            // Guardar polaca
            generarAssembler();
            printf("\n##########\nCOMPILACION CORRECTA\n##########\n");
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
            PonerEnPolaca("WRITE");

      }
      |WRITE CTE_FLOAT
	    {
			  char* aux = guardar_cte_float($<real_val>2);
            PonerEnPolaca(aux);
            PonerEnPolaca("WRITE");


	    }
      |WRITE CTE_STRING
	    {
                
            char* nombre_cte_string = guardar_cte_string($<str_val>2);
            PonerEnPolaca(nombre_cte_string);
            PonerEnPolaca("WRITE");


      }
      |WRITE ID
	    {
			if(!existe_simbolo($<str_val>2)){
                        printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                        yyerror("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n");
			}
                  PonerEnPolaca($<str_val>2);
                  PonerEnPolaca("WRITE");
	    }		  
	  ;

entrada:
      READ ID{
            if(!existe_simbolo($<str_val>2)){
                  printf("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n",$<str_val>2);
                  yyerror("NO SE DECLARO LA VARIABLE - %s - EN LA SECCION DE DEFINICIONES\n");
		}
            PonerEnPolaca($<str_val>2);
            PonerEnPolaca("READ");
      }
	  ;

asignacion: 
      ID ASIG {
            char aux[strlen($<str_val>1) + 1];
            strcpy(aux, $<str_val>1);
            PonerEnPolaca(aux);            
      }expresion{
            PonerEnPolaca("ASIG");
      }
      |ID ASIG CTE_STRING
	  {
            char aux[strlen($<str_val>1) + 1];
            strcpy(aux, $<str_val>1);
            PonerEnPolaca(aux); 
		aux[strlen($<str_val>3) + 1];
            strcpy(aux, $<str_val>3);
            PonerEnPolaca(aux); 
            PonerEnPolaca("ASIG");
	  }
	  ;
	  
asignacion_mult:
	  lista ASIGMULT expresion{
            PonerEnPolaca("ASIG");
            char* info = sacarDePila(pilaAsigMult);
            while(!pilaVacia(pilaAsigMult)){
		      // printf("\n###\t INFO*:\t\t\t %p\t###\n",&info);
		       sprintf(stderr,"\n###\t INFO:\t\t\t %s\t###\n",info);
                PonerEnPolaca(info);
		      //printf("\n###\t AUXVAR:\t\t\t %s\t###\n",auxVar);
                PonerEnPolaca(auxVar);
                PonerEnPolaca("ASIG");
                info = sacarDePila(pilaAsigMult);
            }
        }
	  ;

lista:
	  ID  ASIGMULT ID{ //CTEE?
              char aux[strlen($<str_val>1) + 1];
              strcpy(aux, $<str_val>1);
              PonerEnPolaca(aux);
              ponerEnPila(pilaAsigMult, aux);
              auxVar = aux;
        }
	  |lista ASIGMULT ID{
              char aux[strlen($<str_val>3) + 1];
              strcpy(aux, $<str_val>3);
              ponerEnPila(pilaAsigMult, aux); //!!

        }
	  ;

iteracion: 
      WHILE PA condicion PC LLA bloque LLC
	  ;

decision:
      IF PA condicion PC LLA bloque LLC{
            int idx = atoi(sacarDePila(pilaComp));
            PonerEnPolacaNro(idx,itoa(GetWriteIDX()) );
      }
      | IF PA condicion PC LLA bloque LLC ELSE LLA bloque LLC{
            int idx = atoi(sacarDePila(pilaComp));
            char * str = (char *)malloc(10);
            itoa(GetWriteIDX(), str,10);
            PonerEnPolacaNro(idx,str );
            int tgt = idx+1;
            idx = atoi(sacarDePila(pilaComp));
            itoa(tgt, str,10);
            PonerEnPolacaNro(idx, str);
      }
	  ;

condicion: 
      comparacion OR comparacion
      |comparacion AND comparacion
      |comparacion
      |NOT comparacion
      |between
	  ;

comparacion: 
      expresion comparador expresion{
            PonerEnPolaca("CMP");
            PonerEnPolaca(sacarDePila(pilaComp));
            // printf("##Poner String en pila##\n");
            char * str = (char *)malloc(10);
            itoa(GetWriteIDX(), str,10);
            PonerEnPolaca("");
            // printf(str);
            // printf("\n####\n");

            ponerEnPila( pilaComp, str); //TODO:revisar
      }
      |expresion
	  ;

between:
      BETWEEN PA ID COMA CA expresion PUNTO_COMA expresion CC PC
	  ;

comparador:
      MAYOR
      {
                  ponerEnPila(pilaComp,  "BLE");
	}
      |MAYOR_IGUAL      
      {
                 ponerEnPila(pilaComp,  "BLT");
	}
      |MENOR_IGUAL      
      {
                  ponerEnPila(pilaComp,  "BGT");
	}
      |MENOR      
      {
                  ponerEnPila(pilaComp,  "BGE");
	}
      |IGUAL      
      {
                  ponerEnPila(pilaComp,  "BNE");
	}
      |DISTINTO      
      {
                  ponerEnPila(pilaComp,  "BEQ");
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
      ID{
            char aux[strlen($<str_val>1) + 1];
            strcpy(aux, $<str_val>1);
            PonerEnPolaca(aux);
            
      }
      |CTE_INT 
	  {
      char* nombre_cte_int = (char*)guardar_cte_int($<int_val>1);
      PonerEnPolaca(nombre_cte_int);
	  }
	  |MENOS CTE_INT 
	  {
      char* nombre_cte_int = (char*)guardar_cte_int($<int_val>2);
      char str[strlen(nombre_cte_int)+1];
      str[0]='-';
      strcat(str,nombre_cte_int);
      PonerEnPolaca(str);

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
      char str[strlen(nombre_cte_float)+1];
      str[0]='-';
      strcat(str,nombre_cte_float);
      PonerEnPolaca(str);

	  }
      |PA expresion PC
	  ;

%%
int main(int argc,char *argv[])
{
      printf("\n");
      printf("\n");
      printf("\n");
      if ((yyin = fopen(argv[1], "rt")) == NULL){
            printf("\nNo se puede abrir el archivo: %s\n", argv[1]);
      }
      else{
            initArray(&array_nombres_variables);
            pila aux = crearPila();
            pilaComp = &aux;
            aux = crearPila();
            pilaAsigMult = &aux;
            crearTabla();
            CrearPolaca();
            yyparse();
            guardar_ts();

            guardarPolaca(argv[2]);
            printf("\n");
            freeArray(&array_nombres_variables);
      }
      printf("\n");
      printf("\n");
      printf("\n");
      fclose(yyin);
      return 0;
}
void yyerror (char const *s) {
   fprintf (stderr, "%s\n", s);
 }