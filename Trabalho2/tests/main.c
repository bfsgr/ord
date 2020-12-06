#include "../include/common.h"
#include "../include/importa.h"

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdbool.h>
#include <cmocka.h>

static void dividir_trivial(void **state){
    FILE *tree = abrir_arquivo("assets/tests/dividir.tree", "rb");
    Pagina p;
    int raiz;
    
    fread(&raiz, sizeof(int), 1, tree);
    le_pagina(tree, &p, raiz);

    Pagina nova;
    inicializa_pagina(&nova);
    int chave_pro, filho_dir, i = 11;

    divide(tree, i, -1, &p, &chave_pro, &filho_dir, &nova);

    assert_int_equal(10, chave_pro);
    assert_int_equal(1, filho_dir);

}

static void insere_unica_pagina(void **state){

    (void) state;
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(dividir_trivial),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}