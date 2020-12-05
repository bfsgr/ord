#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define __COMMMON_INC

#define ORDEM 5
#define MAX_DIG 10

typedef struct {
    int n;
    int chaves[ORDEM - 1]; 
    int filhos[ORDEM];
} Pagina ;


typedef struct {
    int n;
    int chaves[ORDEM]; 
    int filhos[ORDEM + 1];
} PaginaAux ;

#define RRN_SIZE sizeof(Pagina)

#define START sizeof(int)

FILE* abrir_arquivo(char *filename, char* modo);
