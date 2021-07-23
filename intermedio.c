#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "globales.h"

#define ERROR -1
#define OK 0

//Pila
typedef struct
{
	char *cadena;
	int cantExpresiones;
}t_info;

typedef struct s_nodoPila{
	t_info info;
	struct s_nodoPila* psig;
}t_nodoPila;

typedef t_nodoPila *t_pila;

//Polaca
nodoPolaca polaca[MAX_TAM_POLACA];


int writeIdxPolaca = 0;
int readIdxPolaca = 0;
///////////////////// DECLARACION DE FUNCIONES /////////////////////
// int yyerrormsj(const char *,enum tipoDeError,enum error, const char*);
//int yyerror();

//Funciones para notacion intermedia
void guardarPolaca();
int PonerEnPolacaNro(int, char *);
int PonerEnPolaca(char *);
void CrearPolaca();
//Funciones para manejo de pilas
void vaciarPila();
t_info* sacarDePila(t_pila*);
t_pila* crearPila();
int ponerEnPila(t_pila*,t_info*);
t_info* topeDePila(t_pila*);

//Notacion intermedia
t_pila pilaAVG;
t_pila pilaIf;
t_pila pilaWhile;
t_pila pilaASM;

/////////////////Pila/////////////////////////////////////////////////////

	t_pila* crearPila()
	{
		// printf("CrearPila\n");
	    t_pila* pp = (t_pila*)malloc(sizeof(t_nodoPila));
		return pp;
	}

	int ponerEnPila(t_pila* pp,t_info* info)
	{
		// printf("poner en Pila\n");

	    t_nodoPila* pn=(t_nodoPila*)malloc(sizeof(t_nodoPila));
	    if(!pn)
	        return 0;
	    pn->info=*info;
	    pn->psig=*pp;
	    *pp=pn;
	    return 1;
	}

	t_info * sacarDePila(t_pila* pp)
	{
		// printf("SacardEPila\n");

		t_info* info = (t_info *) malloc(sizeof(t_info));
	    if(!*pp){
	    	return NULL;
	    }
	    *info=(*pp)->info;
	    *pp=(*pp)->psig;
	    return info;

	}

	void vaciarPila(t_pila* pp)
	{
		// printf("VaciarPila\n");

	    t_nodoPila* pn;
	    while(*pp)
	    {
	        pn=*pp;
	        *pp=(*pp)->psig;
	        free(pn);
	    }
	}

	t_info* topeDePila(t_pila* pila){
		return &((*pila)->info);
	}
                  
	int PonerStringEnPila(t_pila* pp,char * str)
	{
		t_info info;
		info.cadena = str;
	    return ponerEnPila(pp, &info);
	}

/////////////////POLACA/////////////////////////////////////////////////////
void CrearPolaca()
{
	writeIdxPolaca = 0;	
	readIdxPolaca= 0;
}

char * PeekPolaca(){
	char * res = (char*)malloc(sizeof(char)*CADENA_MAXIMA);
	if(readIdxPolaca < 0 || readIdxPolaca>=MAX_TAM_POLACA || readIdxPolaca>=writeIdxPolaca)
		return NULL;
	strcpy(res, polaca[readIdxPolaca].val);	
	return res;
}

char * SacarPolaca(){
	char * res = PeekPolaca();
	if(res == NULL) readIdxPolaca++;
	return res;

}
int PonerEnPolaca( char *cadena)
{

	if(writeIdxPolaca >= MAX_TAM_POLACA  ){
		printf("ponerEnPolaca: #POL_FULL.\n");
		return ERROR;
	}	
	if(strlen(cadena)>= CADENA_MAXIMA){
		printf("ponerEnPolaca: #LONG_OP\n");
		return ERROR;
	}
	printf(cadena);
	printf(" ");
	strcpy(polaca[writeIdxPolaca].val, cadena);
	writeIdxPolaca++;
	return OK;
}

int PonerEnPolacaNro(int pos, char *cadena)
{
	if(pos < 0 || pos >= MAX_TAM_POLACA||strlen(cadena)>CADENA_MAXIMA)
	return ERROR;

	strcpy(polaca[pos].val, cadena);
	return OK;
}

void guardarPolaca( char *cadena){

	FILE*fp=fopen(cadena,"w+");
	if(!fp){
		printf("Error al crear %s", cadena);
		return;
	}
	
	int cont = 0;
	while(cont < writeIdxPolaca){
		fprintf(fp, "%s ", polaca[cont].val);
		cont++;
	}
	fclose(fp);
}

// char* obtenerSalto(enum tipoSalto tipo){
// 	switch(tipo){
// 		case normal:
// 			if(strcmp(ultimoComparador,"==")==0)
// 				return("BEQ");
// 			if(strcmp(ultimoComparador,">")==0)
// 				return("BGT");
// 			if(strcmp(ultimoComparador,"<")==0)
// 				return("BLT");
// 			if(strcmp(ultimoComparador,">=")==0)
// 				return("BGE");
// 			if(strcmp(ultimoComparador,"<=")==0)
// 				return("BLE");
// 			if(strcmp(ultimoComparador,"!=")==0)
// 				return("BNE");
// 			break;

// 		case inverso:
// 			if(strcmp(ultimoComparador,"==")==0)
// 				return("BNE");
// 			if(strcmp(ultimoComparador,">")==0)
// 				return("BLE");
// 			if(strcmp(ultimoComparador,"<")==0)
// 				return("BGE");
// 			if(strcmp(ultimoComparador,">=")==0)
// 				return("BLT");
// 			if(strcmp(ultimoComparador,"<=")==0)
// 				return("BGT");
// 			if(strcmp(ultimoComparador,"!=")==0)
// 				return("BEQ");
// 			break;
// 	}
// }

// char* obtenerSalto2(char* comparador,enum tipoSalto tipo){
// 	switch(tipo){
// 		case normal:
// 			if(strcmp(comparador,"==")==0)
// 				return("BEQ");
// 			if(strcmp(comparador,">")==0)
// 				return("BGT");
// 			if(strcmp(comparador,"<")==0)
// 				return("BLT");
// 			if(strcmp(comparador,">=")==0)
// 				return("BGE");
// 			if(strcmp(comparador,"<=")==0)
// 				return("BLE");
// 			if(strcmp(comparador,"!=")==0)
// 				return("BNE");
// 			break;

// 		case inverso:
// 			if(strcmp(comparador,"==")==0)
// 				return("BNE");
// 			if(strcmp(comparador,">")==0)
// 				return("BLE");
// 			if(strcmp(comparador,"<")==0)
// 				return("BGE");
// 			if(strcmp(comparador,">=")==0)
// 				return("BLT");
// 			if(strcmp(comparador,"<=")==0)
// 				return("BGT");
// 			if(strcmp(comparador,"!=")==0)
// 				return("BEQ");
// 			break;
// 	}
// }