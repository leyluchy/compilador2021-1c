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
typedef struct {
	char* nodos[MAX_TAM_POLACA];
	int top;
} pila;

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

/////////////////Pila/////////////////////////////////////////////////////
	pila crearPila(){
		pila p;
		p.top = -1;
		return p;
	}
	void ponerEnPila(pila* p, char* nodo)
	{
		p->top++;
		p->nodos[p->top] = nodo;
	}

	char* sacarDePila(pila* p)
	{
		char* aux = p->nodos[p->top];
		p->top--;
		return aux;
	}


	char* topeDePila(pila* p){
		return p->nodos[p->top];
	}

	int pilaVacia(pila* p){
		return p->top < 0;
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

int GetWriteIDX(){
	return writeIdxPolaca;
}

int PonerEnPolacaNro(int pos, char *cadena)
{
	if(pos < 0 || pos >= MAX_TAM_POLACA||strlen(cadena)>CADENA_MAXIMA)
	return ERROR;
	printf(cadena);
	printf(" ");
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