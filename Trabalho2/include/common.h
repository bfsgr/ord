#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define __COMMMON_INC

#define ORDEM 4
#define MAX_DIG 10

typedef struct {
    int esq;
    int chave; 
    int dir;
} Chave ;

typedef struct {
    int n;
    Chave chaves[ORDEM - 1]; 
} Pagina ;

#define RRN_SIZE sizeof(Pagina)

#define START sizeof(int)

FILE* abrir_arquivo(char *filename, char* modo);
