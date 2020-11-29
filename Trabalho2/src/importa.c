#include "../include/common.h"
#include "../include/importa.h"

void importa(char* filename) {
    FILE* btree = abrir_arquivo("btree.dat", "w+b");
    FILE* chaves = abrir_arquivo(filename, "r");


    //le uma chave inicial do arquivo de chaves
    Chave key = gera_chave(le_linha(chaves)); 

    //cria uma página e a inicialize
    Pagina pag;
    //seta a memória da pagina como -1 e n = 0
    inicializa_pagina(&pag);
    //registra a primeira chave na pagina
    pag.chaves[0] = key;
    pag.n = 1;
    //salva a pagina no disco
    fwrite(&pag, RRN_SIZE, 1, btree);


    while(feof(chaves) == 0){
        int num = le_linha(chaves);
        key = gera_chave(num);

        int rrn = busca(btree, num);
        inserir(btree, rrn, key);
    }

    fclose(btree);
    fclose(chaves);
}

void divide(FILE* tree, Pagina *p, int *dir, int *esq){
    const int meio = (ORDEM - 1) / 2;
    int offset;
    Pagina p1, p2;
    inicializa_pagina(&p1);
    inicializa_pagina(&p2);

    for(int i = 0; i < meio; i++){
        p1.chaves[i] = p->chaves[i];
        p1.n++;
    }

    for(int i = meio; i < ORDEM - 1; i++){
        p2.chaves[i - meio] = p->chaves[i];
        p2.n++;
    }

    fseek(tree, 0, SEEK_END);
    offset = ftell(tree) / RRN_SIZE;
    *esq = offset;
    fwrite(&p1, RRN_SIZE, 1, tree);
    offset = ftell(tree) / RRN_SIZE;
    *dir = offset;
    fwrite(&p2, RRN_SIZE, 1, tree);
}

void promove(FILE* tree, Pagina *p, int raiz, int *dir, int *esq){
    Pagina pdir;

    fseek(tree, RRN_SIZE * (*dir), SEEK_SET);
    fread(&pdir, RRN_SIZE, 1, tree);

    inicializa_pagina(p);

    p->chaves[0] = pdir.chaves[0];
    p->n = 1;

    for(int i = 0; i < ORDEM - 2; i++){
        pdir.chaves[i] = pdir.chaves[i + 1];
    }
    pdir.n--;

    p->chaves[0].dir = *dir;
    p->chaves[0].esq = *esq;

    fseek(tree, RRN_SIZE * raiz, SEEK_SET);
    fwrite(p, RRN_SIZE, 1, tree);
    fseek(tree, RRN_SIZE * (*dir), SEEK_SET);
    fwrite(&pdir, RRN_SIZE, 1, tree);
}

void inserir(FILE* tree, int rrn, Chave key){
    Pagina p;
    fseek(tree, rrn * RRN_SIZE, SEEK_SET);
    fread(&p, RRN_SIZE, 1, tree);    

    int pos = 0, i = 0;

    while(i < ORDEM - 1 && key.chave > p.chaves[i].chave && p.chaves[i].chave != -1){
        i++;
        pos++;
    }

    for(i = ORDEM - 2; i > pos; i--){
        p.chaves[i] = p.chaves[i - 1];
    }

    p.chaves[pos] = key;
    p.n = p.n + 1;

    fseek(tree, rrn * RRN_SIZE, SEEK_SET);
    fwrite(&p, RRN_SIZE, 1, tree);
}

int busca(FILE* tree, int chave){
    Pagina p;
    //volta para offset 0
    rewind(tree);
    //leia umam página do arquivo
    fread(&p, RRN_SIZE, 1, tree);

    return busca_pagina(tree, &p, chave, 0);
}

int busca_pagina(FILE* tree, Pagina *p, int chave, int rrn){
    if(p != NULL){
        int i = 0;

        while(i < ORDEM - 1 && chave > p->chaves[i].chave && p->chaves[i].chave != -1){
            i++;
        }

        if(chave < p->chaves[i].chave)  {
            //verificar filho esquerdo do elemento em POS
            if(p->chaves[i].esq != -1){
                rrn = p->chaves[i].esq;
                fseek(tree, rrn * RRN_SIZE, SEEK_SET);
                fread(p, RRN_SIZE, 1, tree);
                return busca_pagina(tree, p, chave, rrn);
            } else if(p->n >= ORDEM - 1) {
                //divisão e promoção
                int dir, esq;
                divide(tree, p, &dir, &esq);
                promove(tree, p, rrn, &dir, &esq);
                return busca_pagina(tree, p, chave, rrn);
            } else {
                return rrn;
            }
        } else {
            if(p->chaves[i - 1].dir != -1){
                rrn = p->chaves[i - 1].dir;
                fseek(tree, rrn * RRN_SIZE, SEEK_SET);
                fread(p, RRN_SIZE, 1, tree);
                return busca_pagina(tree, p, chave, rrn);
            } else if(p->n >= ORDEM - 1) {
                //divisão e promoção
                int dir, esq;
                divide(tree, p, &dir, &esq);
                promove(tree, p, rrn, &dir, &esq);

                return busca_pagina(tree, p, chave, rrn);
            } else {
                return rrn;
            }
        }

    } else {
        return -1;
    }
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