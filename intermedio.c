#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

//Pila
typedef struct
{
	char *cadena;
	int cantExpresiones;
	int salto1;
	int salto2;
	int nro;
	enum and_or andOr;
	enum tipoDato tipo;
}t_info;

typedef struct s_nodoPila{
	t_info info;
	struct s_nodoPila* psig;
}t_nodoPila;

typedef t_nodoPila *t_pila;

//Polaca
typedef struct
{
	char cadena[CADENA_MAXIMA];
	int nro;
}t_infoPolaca;

typedef struct s_nodoPolaca{
	t_infoPolaca info;
	struct s_nodoPolaca* psig;
}t_nodoPolaca;

typedef t_nodoPolaca *t_polaca;

///////////////////// DECLARACION DE FUNCIONES /////////////////////
int yyerrormsj(const char *,enum tipoDeError,enum error, const char*);
int yyerror();

//Funciones para notacion intermedia
void guardarPolaca(t_polaca*);
int ponerEnPolacaNro(t_polaca*,int, char *);
int ponerEnPolaca(t_polaca*, char *);
void crearPolaca(t_polaca*);
char* obtenerSalto(enum tipoSalto);
char* obtenerSalto2(char*,enum tipoSalto);
//Funciones para manejo de pilas
void vaciarPila(t_pila*);
t_info* sacarDePila(t_pila*);
void crearPila(t_pila*);
int ponerEnPila(t_pila*,t_info*);
t_info* topeDePila(t_pila*);

//Notacion intermedia
t_polaca polaca;
t_pila pilaAVG;
t_pila pilaIf;
t_pila pilaWhile;
t_pila pilaASM;

/////////////////Pila/////////////////////////////////////////////////////

	void crearPila(t_pila* pp)
	{
	    *pp=NULL;
	}

	int ponerEnPila(t_pila* pp,t_info* info)
	{
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

/////////////////POLACA/////////////////////////////////////////////////////
void crearPolaca(t_polaca* pp)
{
	*pp=NULL;
}

char * sacarDePolaca(t_polaca * pp){
	t_nodoPolaca* nodo;
	t_nodoPolaca* anterior;
	char * cadena = (char*)malloc(sizeof(char)*CADENA_MAXIMA);;
	nodo = *pp;

	while(nodo->psig){
		anterior = nodo;
		nodo = nodo->psig;
	}

	anterior->psig=NULL;
	strcpy(cadena, nodo->info.cadena);
	free(nodo);
	return cadena;
}

int ponerEnPolaca(t_polaca* pp, char *cadena)
{
	t_nodoPolaca* pn = (t_nodoPolaca*)malloc(sizeof(t_nodoPolaca));
	if(!pn){
		printf("ponerEnPolaca: Error al solicitar memoria (pn).\n");
		return ERROR;
	}
	t_nodoPolaca* aux;
	strcpy(pn->info.cadena,cadena);
	pn->info.nro=contadorPolaca++;
	pn->psig=NULL;
	if(!*pp){
		*pp=pn;
		return OK;
	}
	else{
		aux=*pp;
		while(aux->psig)
			aux=aux->psig;
		aux->psig=pn;
		return OK;
	}
}

int ponerEnPolacaNro(t_polaca* pp,int pos, char *cadena)
{
	t_nodoPolaca* aux;
	aux=*pp;
	while(aux!=NULL && aux->info.nro<pos){
		aux=aux->psig;
	}
	if(aux->info.nro==pos){
		strcpy(aux->info.cadena,cadena);
		return OK;
	}
	else{
		printf("NO ENCONTRADO\n");
		return ERROR;
	}
	return ERROR;
}

void guardarPolaca(t_polaca *pp){
	FILE*pt=fopen("intermedia.txt","w+");
	t_nodoPolaca* pn;
	if(!pt){
		printf("Error al crear la tabla de simbolos\n");
		return;
	}
	while(*pp)
	{
		pn=*pp;
		fprintf(pt, "%s\n",pn->info.cadena);
		*pp=(*pp)->psig;
		//TODO: Revisar si este free afecta al funcionamiento del compilador en W7
		free(pn);
	}
	fclose(pt);
}

char* obtenerSalto(enum tipoSalto tipo){
	switch(tipo){
		case normal:
			if(strcmp(ultimoComparador,"==")==0)
				return("BEQ");
			if(strcmp(ultimoComparador,">")==0)
				return("BGT");
			if(strcmp(ultimoComparador,"<")==0)
				return("BLT");
			if(strcmp(ultimoComparador,">=")==0)
				return("BGE");
			if(strcmp(ultimoComparador,"<=")==0)
				return("BLE");
			if(strcmp(ultimoComparador,"!=")==0)
				return("BNE");
			break;

		case inverso:
			if(strcmp(ultimoComparador,"==")==0)
				return("BNE");
			if(strcmp(ultimoComparador,">")==0)
				return("BLE");
			if(strcmp(ultimoComparador,"<")==0)
				return("BGE");
			if(strcmp(ultimoComparador,">=")==0)
				return("BLT");
			if(strcmp(ultimoComparador,"<=")==0)
				return("BGT");
			if(strcmp(ultimoComparador,"!=")==0)
				return("BEQ");
			break;
	}
}

char* obtenerSalto2(char* comparador,enum tipoSalto tipo){
	switch(tipo){
		case normal:
			if(strcmp(comparador,"==")==0)
				return("BEQ");
			if(strcmp(comparador,">")==0)
				return("BGT");
			if(strcmp(comparador,"<")==0)
				return("BLT");
			if(strcmp(comparador,">=")==0)
				return("BGE");
			if(strcmp(comparador,"<=")==0)
				return("BLE");
			if(strcmp(comparador,"!=")==0)
				return("BNE");
			break;

		case inverso:
			if(strcmp(comparador,"==")==0)
				return("BNE");
			if(strcmp(comparador,">")==0)
				return("BLE");
			if(strcmp(comparador,"<")==0)
				return("BGE");
			if(strcmp(comparador,">=")==0)
				return("BLT");
			if(strcmp(comparador,"<=")==0)
				return("BGT");
			if(strcmp(comparador,"!=")==0)
				return("BEQ");
			break;
	}
}