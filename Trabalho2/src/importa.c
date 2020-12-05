#include "../include/common.h"
#include "../include/importa.h"

void importa(char* filename) {
    FILE* btree = abrir_arquivo("btree.dat", "w+b");
    FILE* chaves = abrir_arquivo(filename, "r");
    int raiz = 0;

    // 1) Inicializar uma página com a primeira chave de $chaves
    int chave = le_linha(chaves);
    Pagina p;
    inicializa_pagina(&p);
    p.chaves[0] = chave;
    p.n++;

    // Escrever o cabeçalho
    fwrite(&raiz, sizeof(int), 1, btree);
    // Escrever a página
    escreve_pagina(btree, &p, raiz);
    chave = le_linha(chaves); 

    // 2) Inserir chave a chave
    while(chave != -1) {
        int filho_pro, chave_pro;
        int resultado = inserir(btree, raiz, chave, &filho_pro, &chave_pro); 

        if(resultado == PROMOCAO){
            Pagina new;
            inicializa_pagina(&new);
            new.chaves[0] = chave_pro;
            new.filhos[0] = raiz;
            new.filhos[1] = filho_pro;
            new.n++;

            fseek(btree, 0, SEEK_END);
            raiz = (ftell(btree) - START) / RRN_SIZE;
            fwrite(&new, RRN_SIZE, 1, btree);

            fseek(btree, 0, SEEK_SET);
            fwrite(&raiz, sizeof(int), 1, btree);
        }

        chave = le_linha(chaves); 
    }

    fclose(btree);
    fclose(chaves);
}

bool busca_na_pagina(int k, void *p, int *pos){
    int i = 0;
    while(i < ((Pagina*)p)->n && k > ((Pagina*)p)->chaves[i] && i + 1 < ORDEM - 1){
        i++;
    }
    *pos = i;

    return ((Pagina*)p)->chaves[i] == k;

} 

int inserir(FILE* tree, int rrn_atual, int k, int *filho_dir_pro, int *chave_promovida) {
    if(rrn_atual == EOF){
        *chave_promovida = k;
        *filho_dir_pro = EOF;
        return PROMOCAO;
    } else {
        Pagina p;
        int pos, retorno;
        le_pagina(tree, &p, rrn_atual);
        //busque esse elemento na pagina
        if(busca_na_pagina(k, &p, &pos)){
            printf("Chave %i duplicada\n", k);
            return ERRO;
        } 

        retorno = inserir(tree, p.filhos[pos], k, filho_dir_pro, chave_promovida);

        if(retorno == SEM_PROMOCAO || retorno == ERRO){
            return retorno;
        } else {
            //tem espaço?
            if(p.n < ORDEM - 1) {
                //desloca
                for(int i = ORDEM - 2; i > pos; i--){
                    p.chaves[i] = p.chaves[i-1];
                }
                for(int i = ORDEM - 1; i > pos; i--){
                    p.filhos[i] = p.filhos[i-1];
                }



                p.chaves[pos] = *chave_promovida;
                int aux = p.filhos[pos + 1];
                p.filhos[pos + 1] = *filho_dir_pro;
                p.filhos[pos] = aux;
                p.n++;

                escreve_pagina(tree, &p, rrn_atual);
                return SEM_PROMOCAO;
            } else {
                Pagina nova;
                inicializa_pagina(&nova);
                //divide e promove
                divide(tree, *chave_promovida, *filho_dir_pro, &p, chave_promovida, filho_dir_pro, &nova);
                escreve_pagina(tree, &p, rrn_atual);
                return PROMOCAO;
            }
        }

    }
}

void divide(FILE* tree, int chave, int dir, Pagina *p, int *chave_pro, int *filho_dir_pro, Pagina *nova_pag){
    PaginaAux aux;
    memset(&aux, -1, sizeof(PaginaAux));
    aux.n = p->n;
    for(int i = 0; i < ORDEM - 1; i++){
        aux.chaves[i] = p->chaves[i];
    }
    for(int i = 0; i < ORDEM; i++){
        aux.filhos[i] = p->filhos[i];
    }
        
    
    int pos;
    busca_na_pagina(chave, &aux, &pos);

    //desloca
    for(int i = ORDEM - 1; i > pos; i--){
        aux.chaves[i] = aux.chaves[i-1];
    }
    for(int i = ORDEM; i > pos; i--){
        aux.filhos[i] = aux.filhos[i-1];
    }

    aux.chaves[pos] = chave;
    aux.filhos[pos + 1] = dir;
    aux.n++;

    *chave_pro = aux.chaves[ORDEM / 2];

    inicializa_pagina(p);
    for(int i = 0; i < ORDEM / 2; i++) {
        p->chaves[i] = aux.chaves[i];
        p->n++;
    }
    for(int i = 0; i < (ORDEM / 2) + 1; i++) {
        p->filhos[i] = aux.filhos[i];
    }

    int j = 0;
    for(int i = (ORDEM / 2) + 1; i < ORDEM; i++){
        nova_pag->chaves[j] = aux.chaves[i];
        nova_pag->n++;
        j++;
    }
    j = 0;
    for(int i = (ORDEM / 2) + 1; i < ORDEM + 1; i++){
        nova_pag->filhos[j] = aux.filhos[i];
        j++;
    }

    
    fseek(tree, 0, SEEK_END);
    *filho_dir_pro = (ftell(tree) - START) / RRN_SIZE;
    fwrite(nova_pag, RRN_SIZE, 1, tree);
}

void le_pagina(FILE* tree, Pagina *p, int rrn){
    fseek(tree, rrn * RRN_SIZE + START, SEEK_SET);
    fread(p, RRN_SIZE, 1, tree);
}

void escreve_pagina(FILE* tree, Pagina *p, int rrn){
    fseek(tree, rrn * RRN_SIZE + START, SEEK_SET);
    fwrite(p, RRN_SIZE, 1, tree);
}

void inicializa_pagina(Pagina *p){
        memset(p, -1, RRN_SIZE);
        p->n = 0;
}


int le_linha(FILE* file){
    if(feof(file) != 0){
        return EOF;
    } else {
        char cur = fgetc(file);
        char str[MAX_DIG];
        memset(str, '\0', MAX_DIG);

        while(cur != '\n' && cur != EOF){
            str[strlen(str)] = cur;
            cur = getc(file); 
        }

        return atoi(str);
    }
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