
#ifndef __COMMMON_INC
    #include "./common.h"
#endif


void importa(char* filename);
void inserir(FILE* tree, Chave k);
int busca(FILE* tree, Chave k);
int busca_pagina(FILE* tree, int rrn, Pagina *p, Chave k);
void inicializa_pagina(Pagina *p);
void le_pagina(FILE* tree, Pagina *p, int rrn);
void escreve_pagina(FILE* tree, Pagina *p);
int le_linha(FILE* file);
Chave gera_chave(int num);