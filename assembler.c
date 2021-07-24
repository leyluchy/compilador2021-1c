#include "assembler.h"
#include "globales.h"
#include "y.tab.h"
#include <stdio.h>

extern simbolo ts[TAM_TABLA];
extern cant_elem_ts;
extern writeIdxPolaca;
extern nodoPolaca polaca[MAX_TAM_POLACA];
simbolo simbolo_busqueda;

char* pilaPolaca[MAX_TAM_POLACA];
int topPila = -1;

void generarAssembler(){
    FILE* arch = fopen("Final.asm", "w");
    if(!arch){
        printf("No pude crear el archivo final.asm\n");
        return;
    }

    escribirInicio(arch);
    generarTabla(arch);
    escribirInicioCodigo(arch);
    escribirCodigo(arch);
    escribirFinal(arch);
    fclose(arch);
}

// Constantes
void escribirInicio(FILE *arch){
    fprintf(arch, "include macros2.asm\ninclude number.asm\n\n.MODEL SMALL\n.386\n.STACK 200h\n\n");
}

void escribirInicioCodigo(FILE* arch){
    fprintf(arch, ".CODE\n\nMOV AX, @DATA\nMOV DS, AX\nFINIT\n\n");
}

void escribirFinal(FILE *arch){
    fprintf(arch, "\nMOV AH, 1\nINT 21h\nMOV AX, 4C00h\nINT 21h\n\nEND\n");
}

// Tabla de simbolos
void generarTabla(FILE *arch){
    fprintf(arch, ".DATA\n");
    fprintf(arch, "NEW_LINE DB 0AH,0DH,'$'\n");
	fprintf(arch, "CWprevio DW ?\n");

    for(int i = 0; i < cant_elem_ts; i++){
        fprintf(arch, "%s ", ts[i].nombre);
        switch(ts[i].tipo_dato){
        case TIPO_CTE_INT:
            fprintf(arch, "dd %s\n", ts[i].valor);
            break;
        case TIPO_CTE_FLOAT:
            fprintf(arch, "dd %s\n", ts[i].valor);
            break;
        case TIPO_CTE_STRING:
            fprintf(arch, "db %s, '$'\n", ts[i].valor);
            break;
        default: //Es una variable int, float o puntero a string
            fprintf(arch, "dd ?\n");
        }
    }

    fprintf(arch, "\n");
}

// Codigo en si
void escribirCodigo(FILE *arch){
    for(int i=0; i < writeIdxPolaca; i++){
        if(polaca[i].val == NULL) {
            printf("ERROR no encontre valor en la posicion %d de la polaca", i);
            return;
        }

        if(strcmp("READ", polaca[i].val) == 0)
            read(arch);
        else if(strcmp("WRITE", polaca[i].val) == 0)
            write(arch);
        else if(strcmp("ASIG", polaca[i].val) == 0)
            asignar(arch);
        else if(strcmp("MAS", polaca[i].val) == 0)
            suma(arch);
        else if(strcmp("MENOS", polaca[i].val) == 0)
            resta(arch);
        else if(strcmp("-", polaca[i].val) == 0)
            cambiar_signo(arch);
        else if(strcmp("POR", polaca[i].val) == 0)
            multiplicacion(arch);
        else if(strcmp("DIVIDIDO", polaca[i].val) == 0)
            division(arch);
        else if(strcmp("DIV", polaca[i].val) == 0)
            division_entera(arch);
        else if(strcmp("MOD", polaca[i].val) == 0)
            modulo(arch);
        else if(strcmp("CMP", polaca[i].val) == 0)
            comparar(arch);
        else if(strcmp("CMP", polaca[i].val) == 0){
            // Escribir salto y etiqueta
            saltar(arch, polaca[i].val, polaca[i+1].val);
            i++;
        } else if(strcmp("ETIQUETA", polaca[i].val) == 0){
            // Escribir etiqueta
            i++;
            etiquetar(arch, polaca[i].val);
        }
        else {
            // Sino es un ID
            //strcpy(temp_id, polaca[i].val); // Lo cargo en variable global
            apilar(polaca[i].val);
            //load(arch); // Decide como cargarlo segun el tipo de dato
        }
    }
}

void apilar(char* nodo){
    topPila++;
    pilaPolaca[topPila] = nodo;
}

char* desapilar() {
    char* aux = pilaPolaca[topPila];
    topPila--;
    return aux;
}

// Auxiliares de codigo
void load(FILE* arch, char* name){
    existe_simbolo(name); // Si existe, rellena simbolo_busqueda

    switch(simbolo_busqueda.tipo_dato){
    case TIPO_CTE_INT:
	case TIPO_INT:
		//FILD n; Donde n es el numero integer en memoria
        fprintf(arch, "FILD %s\n", simbolo_busqueda.nombre);
        break;
    case TIPO_CTE_FLOAT:
	case TIPO_FLOAT:
		//FLD n; Donde n es el numero float en memoria
        fprintf(arch, "FLD %s\n", simbolo_busqueda.nombre);
        break;
	// Las strings no se cargan en assembler ahora. Solo se usan seguidas de un write, la dejo en temp_id
	}
}

void suma(FILE* arch){
    char* op = desapilar();
    if (strcmp(op, "@stack") != 0)    
        load(arch, op);
    op = desapilar();
    if (strcmp(op, "@stack") != 0)    
        load(arch, op);
    fprintf(arch, "FADD\n");
    apilar("@stack");
}

void resta(FILE* arch){
    char* op2 = desapilar();
    char* op1 = desapilar();
    int op1inStack, op2inStack = 1;
    if (strcmp(op1, "@stack") != 0) {   
        load(arch, op1);
        op1inStack = 0;
    }
    if (strcmp(op2, "@stack") != 0) { 
        load(arch, op2);
        op2inStack = 0;
    }
    if (!op1inStack && op2inStack)
        // Si op1 no estaba en copro pero op2 si, quedaron al reves
        fprintf(arch, "FCHS\n");
    fprintf(arch, "FSUB\n");
    apilar("@stack");
}

void cambiar_signo(FILE* arch){
    char* op = desapilar();
    if (strcmp(op, "@stack") != 0)    
        load(arch, op);
    fprintf(arch, "FCHS\n");
}

void multiplicacion(FILE* arch){
    char* op = desapilar();
    if (strcmp(op, "@stack") != 0)    
        load(arch, op);
    op = desapilar();
    if (strcmp(op, "@stack") != 0)    
        load(arch, op);
    fprintf(arch, "FMUL\n");
    apilar("@stack");
}

void division(FILE* arch){
    char* op2 = desapilar();
    char* op1 = desapilar();
    int op1inStack, op2inStack = 1;
    if (strcmp(op1, "@stack") != 0) {   
        load(arch, op1);
        op1inStack = 0;
    }
    if (strcmp(op2, "@stack") != 0) { 
        load(arch, op2);
        op2inStack = 0;
    }
    if (!op1inStack && op2inStack)
        // Si op1 no estaba en copro pero op2 si, quedaron al reves
        fprintf(arch, "FCHS\n");
    fprintf(arch, "FDIV\n");
    apilar("@stack");
}

void division_entera(FILE* arch){
    // DIV da quotient e EAX
    // fprintf(arch, "DIV\n");
    // fprintf(arch, "MOV %s, EAX\n");
}

void modulo(FILE* arch){
    // DIV da remainder o modulo en EDX
}

void write(FILE* arch){
	existe_simbolo(desapilar()); // Si existe, rellena simbolo_busqueda
	switch(simbolo_busqueda.tipo_dato){
    case TIPO_CTE_INT:
	case TIPO_INT:
		fprintf(arch, "DisplayInteger %s\n", simbolo_busqueda.nombre);
		break;
    case TIPO_CTE_FLOAT:
	case TIPO_FLOAT:
		fprintf(arch, "DisplayFloat %s,2\n", simbolo_busqueda.nombre);
		break;
	case TIPO_STRING:
		fprintf(arch, "MOV EBX, %s\ndisplayString [EBX]\n", simbolo_busqueda.nombre);
		break;
	case TIPO_CTE_STRING:
		fprintf(arch, "displayString %s\n", simbolo_busqueda.nombre);
		break;
	}
    
	fprintf(arch, "displayString NEW_LINE\n");
	fprintf(arch, "\n");
}

void read(FILE* arch){
    existe_simbolo(desapilar()); // Si existe, rellena simbolo_busqueda
	switch(simbolo_busqueda.tipo_dato){
	case TIPO_INT:
		fprintf(arch, "getInteger %s\n", simbolo_busqueda.nombre);
		break;
	case TIPO_FLOAT:
		fprintf(arch, "getFloat %s\n", simbolo_busqueda.nombre);
		break;
	case TIPO_STRING:
		fprintf(arch, "getString %s\n", simbolo_busqueda.nombre);
	}
	fprintf(arch, "\n");
}

void asignar(FILE* arch){
    char* origen = desapilar();
    char* destino = desapilar();
    existe_simbolo(destino); // Si existe, rellena simbolo_busqueda
	switch(simbolo_busqueda.tipo_dato){
	case TIPO_INT:
        if (strcmp("@stack", origen) != 0)
            load(arch, origen);
        else // ya esta en copro, lo redondeo a entero
            fprintf(arch, "FSTCW CWprevio ;Guardo Control Word del copro\nOR CWprevio, 0400h ;Preparo Control Word seteando RC con redondeo hacia abajo\nFLDCW CWprevio ;Cargo nueva Control Word\n");
		fprintf(arch, "FISTP %s", destino);
		break;
	case TIPO_FLOAT:
        if (strcmp("@stack", origen) != 0)
            load(arch, origen);
		fprintf(arch, "FSTP %s", destino);
		break;
	case TIPO_STRING:
		fprintf(arch, "LEA EAX, %s\nMOV %s, EAX", origen, destino);
		break;
	}
    fprintf(arch, "\n");
}

void comparar(FILE *arch){
	fprintf(arch, "FXCH\nFCOMP\nFSTSW AX\nSAHF\n");
}

void saltar(FILE* arch, char* salto, char* etiqueta){
    fprintf(arch, "%s %s\n", salto, etiqueta);
}

void etiquetar(FILE* arch, char* etiqueta){
    fprintf(arch, "%s:\n", etiqueta);
}