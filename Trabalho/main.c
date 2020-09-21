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

//Importa os dados de um arquivo arbitrário e os escreve em dados.dat
//Retorno:
//  TRUE - se a importação ocorrer com sucesso
//  FALSE - se algum erro acontecer
bool importa(char* filename){
    FILE* dados = abrir_arquivo(filename, "rb");
    FILE* dat = abrir_arquivo("dados.dat", "w");

    if(!cria_header(dat)){
        return false;
    }
    
    char bloco[BLOCO];

    while(le_bloco(dados, bloco) > 0){
        char *l = strtok(bloco, DELIM_REG);

        while(l != NULL){
            if(!escreve(dat, l)){
                return false;
            }
            l = strtok(NULL, DELIM_REG);
        }
    }

    fclose(dat);
    fclose(dados);
    return true;
}

//Escreve um registro arbitrário na cabeça de leitura atual do arquivo de dados
//Prefixa o registro com o seu tamanho
//Retorno:
//  TRUE - se o registro for escrito com sucesso
//  FALSE -se algum erro acontecer na escrita
bool escreve(FILE* arquivo, char *registro){
    short size = strlen(registro);
    if(fwrite(&size, sizeof(size), 1, arquivo) != 1){
        printf("Erro de escrita - (tamanho)\n");
        return false;
    }
    if(fputs(registro, arquivo) == EOF) {
        printf("Erro de escrita - (registro)\n");
        return false;
    }
    return true;
}

//Le um bloco de dados do arquivo para um buffer de tamanho BLOCO
//Retorna o número de bytes lidos do arquivo
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

//Cria um header de 8 bytes para o manuseio da LED
//Retorna TRUE se o header for criado com sucesso
//Retorna FALSE se ocorrer um problema de escrita
bool cria_header(FILE* arquivo){
    long head = 0;
    if(fwrite(&head, sizeof(head), 1, arquivo) != 1){
        printf("Erro ao criar o header.\n");
        return false;
    }
    return true;
}

//Abre um arquivo dado o modo
//Retorna um ponteiro para o FILE stream
//Em caso de erro encerra a execução do programa com código 1
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