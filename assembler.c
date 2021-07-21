#include "assembler.h"
#include "globales.h"
#include <stdio.h>

extern simbolo ts[TAM_TABLA];
extern cant_elem_ts;

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

    // TODO: escribir tabla de simbolos
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
            fprintf(arch, "db \"%s\", '$'\n", ts[i].valor);
            break;
        default: //Es una variable int, float o puntero a string
            fprintf(arch, "dd ?\n");
        }
    }

    fprintf(arch, "\n");
}

void escribirCodigo(FILE *arch){
    //TODO: escribir todo el codigo de la polaca
}