#include "assembler.h"
#include "globales.h"
#include "y.tab.h"
#include <stdio.h>

extern simbolo ts[TAM_TABLA];
extern cant_elem_ts;
extern writeIdxPolaca;
extern nodoPolaca polaca[MAX_TAM_POLACA];
simbolo simbolo_busqueda;

char temp_id[CADENA_MAXIMA];

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
        else {
            // Sino es un ID
            strcpy(temp_id, polaca[i].val); // Lo cargo en variable global
            load(arch); // Decide como cargarlo segun el tipo de dato
        }
    }
}

// Auxiliares de codigo
void load(FILE* arch){
    existe_simbolo(temp_id); // Si existe, rellena simbolo_busqueda

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
    fprintf(arch, "FADD\n");
}

void resta(FILE* arch){
    fprintf(arch, "FSUB\n");
}

void cambiar_signo(FILE* arch){
    fprintf(arch, "FCHS\n");
}

void multiplicacion(FILE* arch){
    fprintf(arch, "FMUL\n");
}

void division(FILE* arch){
    fprintf(arch, "FDIV\n");
}

void division_entera(FILE* arch){
    // DIV da quotient e EAX
}

void modulo(FILE* arch){
    // DIV da remainder o modulo en EDX
}

void write(FILE* arch){
	existe_simbolo(temp_id); // Si existe, rellena simbolo_busqueda
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
    existe_simbolo(temp_id); // Si existe, rellena simbolo_busqueda
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