#include "../include/importa.h"
#include "../include/imprime.h"

void imprime(){
    FILE *btree = abrir_arquivo("btree.dat", "rb");

    int raiz, paginas = 1, chaves = 0;
    fread(&raiz, START, 1, btree);

    printf("-- RAIZ --\n");
    caminha(btree, raiz, -1, &paginas, &chaves); 

    printf("\n\n-----");
    printf("Estatísticas da Árvore-B:\n");
    printf("> Altura: %i\n", altura_calc(btree, raiz) - 1);
    printf("> Número de Chaves: %i\n", chaves);
    printf("> Número de Páginas: %i\n", paginas);
    printf("> Taxa de ocupação: %.2f%%\n", ( (float)chaves / ((ORDEM - 1) * (float) paginas)) * 100.0);


    fclose(btree);
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

void caminha(FILE* tree, int rrn_dir, int rrn_esq, int *paginas, int *chaves){
    Pagina pdir, pesq;
    if(rrn_dir != -1) {
        le_pagina(tree, &pdir, rrn_dir);
        printa_pagina(&pdir, rrn_dir);
        *chaves += pdir.n;

        int i = 0;

        while(i < ORDEM && pdir.filhos[i] >= 0){
            if(i + 1 < ORDEM - 1 && pdir.filhos[i + 1] != -1){
                *paginas += 2;
                caminha(tree, pdir.filhos[i + 1], pdir.filhos[i], paginas, chaves);
                i += 2;
            } else {
                *paginas += 1;
                caminha(tree, pdir.filhos[i], -1, paginas, chaves);
                i += 1;
            }
        }

    }
    if(rrn_esq != -1){
        le_pagina(tree, &pesq, rrn_esq);
        printa_pagina(&pesq, rrn_esq);
        *chaves += pesq.n;
        
        int i = 0;

        while(i < ORDEM && pesq.filhos[i] >= 0){
            if(i + 1 < ORDEM - 1 && pesq.filhos[i + 1] != -1){
                *paginas += 2;
                caminha(tree, pesq.filhos[i + 1], pesq.filhos[i], paginas, chaves);
                i += 2;
            } else {
                *paginas += 1;
                caminha(tree, pesq.filhos[i], -1, paginas, chaves);
                i += 1;
            }
        }
    }
}

void printa_pagina(Pagina *p, int rrn){
    printf("RRN: %i\n", rrn);
    printf("Chaves: ");

    for(int i = 0; i < p->n; i++){
        printf("%i ", p->chaves[i]);
    }
    printf("\nFilhos: ");


    for(int i = 0; i <= p->n; i++){
        printf("%i | ", p->filhos[i]);
    }
    printf("\n\n");

}