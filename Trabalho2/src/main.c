#include "../include/common.h"
#include "../include/importa.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-c|-p) (nome_arquivo | '')\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-c") == 0 && argc == 3) {
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);
        importa(argv[2]);
        printf("Importação realizada com sucesso!\n");
        
    } else if (strcmp(argv[1], "-p") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n\n", argv[2]);
        printf("Execução finalizada com sucesso!\n");

    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
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