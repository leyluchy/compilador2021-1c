#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>



#define INITIAL_CAPACITY 1

typedef struct {
  char **array;
  size_t used;
  size_t size;
} Array;

Array array_nombres_variables;


void initArray(Array *a);
void insertArray(Array *a, char* element);
void freeArray(Array *a);


void initArray(Array *a) {
  a->array = malloc(INITIAL_CAPACITY * sizeof(char*));
  a->used = 0;
  a->size = INITIAL_CAPACITY;
}

void insertArray(Array *a, char* element) {
  // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
  // Therefore a->used can go up to a->size 
  if (a->used == a->size) {
    a->size *= 2;
    a->array = realloc(a->array, a->size * sizeof(char*));
  }
  a->array[a->used++] = element;
}

void freeArray(Array *a) {
  free(a->array);
  a->array = NULL;
  a->used = a->size = 0;
}