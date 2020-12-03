#include "../include/common.h"
#include "../include/importa.h"

void importa(char* filename) {
    FILE* btree = abrir_arquivo("btree.dat", "w+b");
    FILE* chaves = abrir_arquivo(filename, "r");
    const int zero = 0;

    // 1) Inicializar uma página com a primeira chave de $chaves
    Chave el = gera_chave(le_linha(chaves));
    Pagina p;
    inicializa_pagina(&p);
    p.chaves[0] = el;
    p.n++;

    // Escrever o cabeçalho
    fwrite(&zero, sizeof(int), 1, btree);
    // Escrever a página
    escreve_pagina(btree, &p);

    // 2) Inserir chave a chave
    while(feof(chaves) == 0) {
        el = gera_chave(le_linha(chaves));

        inserir(btree, el); 
    }

    fclose(btree);
    fclose(chaves);
}

void inserir(FILE* tree, Chave k){
    int rrn = busca(tree, k);
    printf("a");

}

int busca(FILE* tree, Chave k){
    // vá para o começo e leia o RRN da raiz
    rewind(tree);
    int raiz;
    fread(&raiz, sizeof(int), 1, tree);
    //vá para o RRN da raiz
    fseek(tree, raiz * RRN_SIZE, SEEK_CUR);

    Pagina p;
    fread(&p, RRN_SIZE, 1, tree);

    return busca_pagina(tree, 0, &p, k);
}

int busca_pagina(FILE* tree, int rrn, Pagina *p, Chave k) {
    //caminhe pelas chaves até encontrar uma chave menor do que k
    int i = 0;
    while(i < p->n && k.chave > p->chaves[i].chave){
        i++;
    }
    //nesse ponto i é o indice da chave no qual k deveria ser inserida
    //há dois casos 
    //i != p->n ==> verificar existência do filho da esquerda
    //i == p->n ==> verificar existência do filho direito
    if(i != p->n){
        if(p->chaves[i].esq != -1){
            int new_rrn = p->chaves[i].esq;
            le_pagina(tree, p, p->chaves[i].esq);
            return busca_pagina(tree, new_rrn, p, k);
        } else {
            return rrn;
        }
    } else {
        if(p->chaves[i - 1].dir != -1){
            int new_rrn = p->chaves[i-1].dir;
            le_pagina(tree, p, p->chaves[i - 1].dir);
            return busca_pagina(tree, new_rrn, p, k);
        } else {
            return rrn;
        }
    }
}

void le_pagina(FILE* tree, Pagina *p, int rrn){
    fseek(tree, rrn * RRN_SIZE + START, SEEK_SET);
    fread(p, RRN_SIZE, 1, tree);
}

void escreve_pagina(FILE* tree, Pagina *p){
    fwrite(p, RRN_SIZE, 1, tree);
}

void inicializa_pagina(Pagina *p){
        memset(p, -1, RRN_SIZE);
        p->n = 0;
}


int le_linha(FILE* file){
    char cur = fgetc(file);
    char str[MAX_DIG];
    memset(str, '\0', MAX_DIG);

    while(cur != '\n' && cur != EOF){
        str[strlen(str)] = cur;
        cur = getc(file); 
    }

    return atoi(str);
}

Chave gera_chave(int num) {
    Chave c = { -1, num, -1  };
    return c;
}


//Abre um arquivo dado o modo
//Retorna um ponteiro para o FILE stream
//Em caso de erro encerra a execução do programa com código 1
FILE* abrir_arquivo(char *filename, char* modo){
    FILE *fp;
    fp = fopen(filename, modo);

    if(fp == NULL || ferror(fp) != 0){ 
        printf("Um erro aconteceu ao abrir o arquivo %s.\n", filename);
        exit(1);
    } else {
        return fp;
    }
}