#ifndef __COMMMON_INC
    #include "common.h"
#endif

void imprime();
void printa_pagina(Pagina *p, int rrn);
void extrai_filhos(Pagina *p,int filhos[]);
void caminha(FILE* tree, int rrn_dir, int rrn_esq, int *paginas, int *chaves, int *altura);
