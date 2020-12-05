
#ifndef __COMMMON_INC
    #include "./common.h"
#endif

#define ERRO (-1)
#define SEM_PROMOCAO (0)
#define PROMOCAO (2)

void importa(char* filename);
int inserir(FILE* tree, int rrn_atual, int k, int *filho_dir_pro, int *chave_promovida);
void divide(FILE* tree, int chave, int dir, Pagina *p, int *chave_pro, int *filho_dir_pro, Pagina *nova_pag);
bool busca_na_pagina(int k, void *p, int *pos);
void inicializa_pagina(Pagina *p);
void le_pagina(FILE* tree, Pagina *p, int rrn);
void escreve_pagina(FILE* tree, Pagina *p, int rrn);
int le_linha(FILE* file);