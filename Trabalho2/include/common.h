#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define ORDEM 4
#define OCUPACAO_MIN (int)ceil((double)ORDEM/2.0)
#define MAX_DIG 10


FILE* abrir_arquivo(char *filename, char* modo);
