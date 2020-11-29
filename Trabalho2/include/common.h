#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define __COMMMON_INC

#define ORDEM 4
#define OCUPACAO_MIN (int)ceil((double)ORDEM/2.0)
#define MAX_DIG 10

typedef struct {
    int dir;
    int chave; 
    int esq;
} Chave ;

typedef struct {
    int n;
    Chave chaves[ORDEM - 1]; 
} Pagina ;

FILE* abrir_arquivo(char *filename, char* modo);
