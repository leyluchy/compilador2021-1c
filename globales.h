#ifndef constantes_h
#define constantes_h

#define TIPO_INT 1
#define TIPO_FLOAT 2
#define TIPO_STRING 3
#define TIPO_CTE_INT 4
#define TIPO_CTE_FLOAT 5
#define TIPO_CTE_STRING 6

#define TAM_TABLA 350
#define TAM_NOMBRE 32


typedef struct {
	char nombre[TAM_NOMBRE];
	int tipo_dato;
	char valor[TAM_NOMBRE];
	int longitud;
} simbolo;

#endif