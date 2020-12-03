#include "../include/imprime.h"

void imprime(){
    FILE *btree = abrir_arquivo("btree.dat", "rb");

    // Pagina raiz;
    // fread(&raiz, RRN_SIZE, 1, btree);

    // printa_pagina(&raiz, 0);

    printf("-- RAIZ --\n");
    caminha(btree, 0, -1);

    fclose(btree);
}

void caminha(FILE* tree, int rrn_dir, int rrn_esq){
    Pagina pdir, pesq;
    if(rrn_dir != -1) {
        fseek(tree, rrn_dir * RRN_SIZE, SEEK_SET);
        fread(&pdir, RRN_SIZE, 1, tree);
        printa_pagina(&pdir, rrn_dir);

        int filhos[ORDEM], i = 0;
        extrai_filhos(&pdir, filhos);

        while(i < ORDEM && filhos[i] >= 0){
            if(filhos[i + 1] != -1){
                caminha(tree, filhos[i + 1], filhos[i]);
                i += 2;
            } else {
                caminha(tree, filhos[i], -1);
                i += 1;
            }
        }

    }
    if(rrn_esq != -1){
        fseek(tree, rrn_esq * RRN_SIZE, SEEK_SET);
        fread(&pesq, RRN_SIZE, 1, tree);
        printa_pagina(&pesq, rrn_esq);
        
        int filhos[ORDEM], i = 0;
        extrai_filhos(&pesq, filhos);

        while(i < ORDEM && filhos[i] >= 0){
            if(filhos[i + 1] != -1){
                caminha(tree, filhos[i + 1], filhos[i]);
                i += 2;
            } else {
                caminha(tree, filhos[i], -1);
                i += 1;
            }
        }
    }

}

void extrai_filhos(Pagina *p, int filhos[]){
    memset(filhos, -1, ORDEM * sizeof(int));
    for(int i = 0; i < p->n; i++){
        filhos[i] = p->chaves[i].esq;
    }
    filhos[p->n] = p->chaves[p->n - 1].dir;
}

void printa_pagina(Pagina *p, int rrn){
    printf("RRN: %i\n", rrn);
    printf("Chaves: ");

    for(int i = 0; i < p->n; i++){
        printf("%i ", p->chaves[i].chave);
    }
    printf("\nFilhos: ");

    int filhos[ORDEM];
    extrai_filhos(p, filhos);

    for(int i = 0; i <= p->n; i++){
        printf("%i | ", filhos[i]);
    }
    printf("\n\n");

}