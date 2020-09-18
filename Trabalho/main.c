#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./operacoes.h"

int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-i") == 0) {
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);
        
        if(importa(argv[2])){
            printf("Importação realizada com sucesso!\n");
        } else {
            printf("Ocorreu um erro durante a importação :(\n");
        }
        
    } else if (strcmp(argv[1], "-e") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n", argv[2]);

    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}

bool importa(char* filename){
    FILE* dados = abrir_arquivo(filename, "rb");
    FILE* dat = abrir_arquivo(strcat(filename, ".dat"), "w");
    
    char bloco[BLOCO];

    while(le_bloco(dados, bloco) > 0){
        char *l = strtok(bloco, DELIM_REG);

        while(l != NULL){
            escreve(dat, l);
            l = strtok(NULL, DELIM_REG);
        }

    }

    fclose(dat);
    fclose(dados);
    return true;
}

bool escreve(FILE* arquivo, char *registro){
    short size = tamanho(registro) + 1;
    fwrite(&size, sizeof(size), 1, arquivo);
    fputc((char) DELIM_FIELD, arquivo);
    fputs(registro, arquivo);
    return true;
}

short tamanho(char *registro){
    return strlen(registro);
}

unsigned int le_bloco(FILE* arquivo, char* buffer){
    unsigned int lidos = fread(buffer, 1, BLOCO, arquivo);
    
    if(lidos < BLOCO){
        if(ferror(arquivo)){
            printf("Erro de leitura do arquivo\n");
            exit(1);
        } else {
            buffer[lidos] = '\0';
            return lidos;
        }
    };

    return lidos;
}

FILE* abrir_arquivo(char *filename, char* modo){
    FILE *fp;
    fp = fopen(filename, modo);

    if(fp == NULL){ 
        printf("Um erro aconteceu ao abrir o arquivo.\n");
        exit(1);
    } else {
        return fp;
    }
}