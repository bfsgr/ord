#include <stdio.h>
#include <stdlib.h>

int input(char *str, int size);
FILE* open_file(char *str);

#define PIPE '|'

typedef struct Pessoa {
    char nome[50];
    char sobrenome[100];
    char endereco[255];
    char cidade[50];
    char estado[3];
    char cep[9];
} Pessoa; 

int main(){ 
    //le o nome do arquivo e o abre;
    printf("Digite o nome do arquivo: ");
    char aux[50];
    input(aux, 50);
    FILE *saida = open_file(aux);

    //auxiliar para coleta de informações
    Pessoa reg;

    //le o sobrenome e seu tamanho
    printf("Digite um sobrenome: ");
    int len = input(reg.sobrenome, 255);

    while(len > 0){
        printf("Nome: ");
        input(reg.nome, 50);
        printf("Endereço: ");
        input(reg.endereco, 255);
        printf("Cidade: ");
        input(reg.cidade, 50);
        printf("Estado (ex: PR, SP): ");
        input(reg.estado, 3);
        printf("CEP (somente números): ");
        input(reg.cep, 9);

        fputs(reg.sobrenome, saida);
        fputc(PIPE, saida);
        fputs(reg.nome, saida);
        fputc(PIPE, saida);
        fputs(reg.endereco, saida);
        fputc(PIPE, saida);
        fputs(reg.cidade, saida);
        fputc(PIPE, saida);
        fputs(reg.estado, saida);
        fputc(PIPE, saida);
        fputs(reg.cep, saida);
        fputc(PIPE, saida);

        printf("Digite um sobrenome: ");
        len = input(reg.sobrenome, 255);
    }
    fclose(saida);
}

int input(char *str, int size){
    int i = 0;
    char c = getchar();
    while(c != '\n'){
        if(size > 1) {
            str[i] = c;
            i++;
            size--;
        }
        c = getchar();
    }
    str[i] = '\0';
    return i;
}


FILE* open_file(char *str) {
    FILE *fp;
    fp = fopen(str, "w");

    if(fp == NULL){ 
        printf("Um erro aconteceu ao abrir o arquivo.\n");
        exit(1);
    } else {
        return fp;
    }
}

