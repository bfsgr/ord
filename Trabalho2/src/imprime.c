#include "../include/imprime.h"

void imprime(){
    FILE *btree = abrir_arquivo("btree.dat", "rb");

    Pagina raiz;
    fread(&raiz, RRN_SIZE, 1, btree);

    printa_pagina(&raiz);

    fclose(btree);
}

void printa_pagina(Pagina *p){

}