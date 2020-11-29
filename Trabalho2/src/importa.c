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

void inserir(FILE* tree, int rrn, Chave key){
    Pagina p;
    fseek(tree, rrn + RRN_SIZE, SEEK_SET);
    fread(&p, RRN_SIZE, 1, tree);    

    int pos = 0, i = 0;

    while(i < ORDEM - 1 && key.chave > p.chaves[i].chave){
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

    return busca_pagina(tree, &p, chave);
}

int busca_pagina(FILE* tree, Pagina *p, int chave){
    static int rrn = 0;
    if(p != NULL){
        int pos = 0, i = 0;

        while(i < ORDEM - 1 && chave > p->chaves[i].chave){
            i++;
            pos++;
        }

        if(pos > ORDEM - 2){
            //verificar filho direito do ultimo elemento
            if(p->chaves[pos - 1].dir != -1){
                rrn = p->chaves[pos].dir;
                fseek(tree, rrn + RRN_SIZE, SEEK_SET);
                fread(p, RRN_SIZE, 1, tree);
                return busca_pagina(tree, p, chave);
            } else {
                //divisão e promoção
            }
        } else  {
            //verificar filho esquerdo do elemento em POS
            if(p->chaves[pos].esq != -1){
                rrn = p->chaves[pos].esq;
                fseek(tree, rrn + RRN_SIZE, SEEK_SET);
                fread(p, RRN_SIZE, 1, tree);
                return busca_pagina(tree, p, chave);
            } else if(p->n >= ORDEM - 1) {
                //divisão e promoção
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