#include "../include/common.h"
#include "../include/importa.h"

//tests are run in a b-tree 4
#define ORDEM 4

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdbool.h>
#include <cmocka.h>

static void busca_pagina_altura2(void **state){
    int raiz;
    Pagina p;
    FILE* tree = fopen("assets/tests/altura2.tree", "rb");
    //le o rrn da raiz e leia a pagina raiz
    fread(&raiz, sizeof(int), 1, tree);
    le_pagina(tree, &p, raiz);

    Chave k = {-1, 7, -1 };
    int result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);
    
    //volte a pagina raiz
    le_pagina(tree, &p, raiz);

    k.chave = 20;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 2);

    fclose(tree);

    (void) state;
}


//no matter what key we try to put in it should always return rrn 0;
static void busca_pagina_unica_pagina(void **state){
    int raiz;
    Pagina p;
    FILE* tree = fopen("assets/tests/unicaPagina.tree", "rb");
    //le o rrn da raiz e leia a pagina raiz
    fread(&raiz, sizeof(int), 1, tree);
    le_pagina(tree, &p, raiz);
    
    //The page has ONE element in it

    //INSERT A LESS THAN ALL ELEMENT
    Chave k = {-1, 8, -1 };
    int result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);

    //INSERT A GREATER THAN ALL ELEMENT
    k.chave = 20;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);

    //change the tree to one that is full
    fclose(tree);
    tree = fopen("assets/tests/unicaPaginaCheia.tree", "rb");
    fread(&raiz, sizeof(int), 1, tree);
    le_pagina(tree, &p, raiz);
    
    //test all cases of insertion in a full page
    k.chave = 1;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);

    k.chave = 18;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);
    
    k.chave = 25;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);

    k.chave = 30;
    result = busca_pagina(tree, raiz, &p, k);
    assert_int_equal(result, 0);

    fclose(tree);

    (void) state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(busca_pagina_unica_pagina),
        cmocka_unit_test(busca_pagina_altura2),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}