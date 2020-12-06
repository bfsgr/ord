#include "../include/importa.h"
#include "../include/imprime.h"

void imprime(){
    FILE *btree = abrir_arquivo("btree.dat", "rb");

    int raiz, paginas = 0, chaves = 0;
    fread(&raiz, START, 1, btree);

    printa(btree, raiz, &paginas, &chaves);

    printf("\n- - - - - - - - - - - - - -\n");
    printf("Estatisticas da Arvore-B:\n");
    printf("> Altura: %i\n", altura_calc(btree, raiz) - 1);
    printf("> Numero de chaves: %i\n", chaves);
    printf("> Numero de paginas: %i\n", paginas);
    printf("> Taxa de ocupacao: %.2f%%\n", ( (float)chaves / ((ORDEM - 1) * (float) paginas)) * 100.0);


    fclose(btree);
}

void printa(FILE* tree, int raiz, int *paginas, int *chaves){
    Pagina p;
    int rrn = 0;
    le_pagina(tree, &p, rrn);
    while(feof(tree) == 0){
        *chaves += p.n;
        *paginas += 1;

        if(rrn == raiz){
            printf("- - - - Pagina Raiz - - - -\n");
        }

        printa_pagina(&p, rrn);

        if(rrn == raiz){
            printf("- - - - - - - - - - - - - -\n");
        }
        printf("\n");
        rrn++;
        le_pagina(tree, &p, rrn);
    }
}

int altura_calc(FILE* tree, int rrn){
    if(rrn == -1){
        return 0;
    } else {
        Pagina p;
        le_pagina(tree, &p, rrn);

        for(int i = 0; i < p.n + 1; i++){
            return 1 + altura_calc(tree, p.filhos[i]);
        }
    }
    return 0;
}

void printa_pagina(Pagina *p, int rrn){
    printf("Pagina %i\n", rrn);
    printf("Chaves: ");

    for(int i = 0; i < p->n - 1; i++){
        printf("%i | ", p->chaves[i]);
    }
    printf("%i", p->chaves[p->n - 1]);
    printf("\nFilhos: ");


    for(int i = 0; i < p->n; i++){
        printf("%i | ", p->filhos[i]);
    }
    printf("%i", p->filhos[p->n]);
    printf("\n");

}