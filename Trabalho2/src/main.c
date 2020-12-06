#include "../include/common.h"
#include "../include/importa.h"
#include "../include/imprime.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-c|-p) (nome_arquivo | '')\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-c") == 0 && argc == 3) {
        printf("Modo de criação ... nome do arquivo = %s\n", argv[2]);
        importa(argv[2]);
        printf("Importação realizada com sucesso!\n");
        
    } else if (strcmp(argv[1], "-p") == 0) {
        imprime();
    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}
