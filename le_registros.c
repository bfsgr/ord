#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Pessoa {
    char nome[50];
    char sobrenome[100];
    char endereco[255];
    char cidade[50];
    char estado[3];
    char cep[9];
} Pessoa; 

#define BUFFER_SIZE 500
#define PIPE "|"

int leia_registro(FILE *fp, char *str, int tam);
FILE* open_file(char *str);
int input(char *str, int size);
void parse(char *str, Pessoa *p);

int main(){
    char filename[50];
    printf("Digite o nome do arquivo a ser aberto: ");
    input(filename, 50);
    FILE *entrada = open_file(filename);

    char buffer[BUFFER_SIZE];
    int lenght = leia_registro(entrada, buffer, BUFFER_SIZE);
    int count = 0;
    while(lenght > 0){
        Pessoa reg;
        parse(buffer, &reg);
        printf("Registro #%i:\n", count++);
        printf("\tNome: %s\n", reg.nome);
        printf("\tSobrenome: %s\n", reg.sobrenome);
        printf("\tEndereço: %s\n", reg.endereco);
        printf("\tEstado: %s\n", reg.estado);
        printf("\tCidade: %s\n", reg.cidade);
        printf("\tCEP: %s\n", reg.cep);

        lenght = leia_registro(entrada, buffer, BUFFER_SIZE);
    }

    fclose(entrada);\
    return 0;
}

int leia_registro(FILE *fp, char *str, int tam){
    short len;

    int status = fread(&len, sizeof(len), 1, fp);

    if(status == 0){
        return 0;
    } 

    if(len < tam) {
        len = fread(str, sizeof(char), len, fp);
        str[len] = '\0';
        return len;
    } else {
        printf("Buffer overflow\n");
        return 0;
    }
}

void parse(char *str, Pessoa *p){
    char *token = strtok(str, PIPE);
    strcpy(p->sobrenome, token);
    token = strtok(NULL, PIPE);
    strcpy(p->nome, token);
    token = strtok(NULL, PIPE);
    strcpy(p->endereco, token);
    token = strtok(NULL, PIPE);
    strcpy(p->cidade, token);
    token = strtok(NULL, PIPE);
    strcpy(p->estado, token);
    token = strtok(NULL, PIPE);
    strcpy(p->cep, token);
}

FILE* open_file(char *str) {
    FILE *fp;
    fp = fopen(str, "r");

    if(fp == NULL){ 
        printf("Um erro aconteceu ao abrir o arquivo.\n");
        exit(1);
    } else {
        return fp;
    }
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