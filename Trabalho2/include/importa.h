
#ifndef __COMMMON_INC
    #include "./common.h"
#endif

#define RRN_SIZE sizeof(Pagina)

void importa(char* filename);
int le_linha(FILE* file);
Chave gera_chave(int num);
int busca(FILE* tree, int chave);
int busca_pagina(FILE* tree, Pagina *p, int chave, int rrn);
void inicializa_pagina(Pagina *p);
void inserir(FILE* tree, int rrn, Chave key);
void divide(FILE* tree, Pagina *p, int *dir, int *esq);
void promove(FILE* tree, Pagina *p, int raiz, int *dir, int *esq);