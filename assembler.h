#ifndef assembler_h
#define assembler_h

#include <stdio.h>

void generarAssembler();
void escribirInicio(FILE* arch);
void escribirInicioCodigo(FILE* arch);
void escribirFinal(FILE *arch);
void generarTabla(FILE* arch);
void escribirCodigo(FILE *arch);

#endif