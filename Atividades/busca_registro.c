#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define PIPE "'|'|"
#define BUFFER_SIZE 500

typedef struct Pessoa {
    char nome[50];
    char sobrenome[100];
    char endereco[255];
    char cidade[50];
    char estado[3];
    char cep[9];
} Pessoa; 

int leia_registro(FILE *fp, char *str, int tam);
FILE* open_file(char *str);
int input(char *str, int size);
void parse(char *str, Pessoa *p);

int main(){
    printf("Digite o nome do arquivo: ");
    char aux[255];
    input(aux, 255);
    FILE *dados = open_file(aux);
    
    
    

    //le o sobrenome e seu tamanho
    printf("Digite um sobrenome: ");
    int len = input(aux, 255);


    if(len > 0){
        bool achou = false;
        char buffer[BUFFER_SIZE];
        int size_reg = leia_registro(dados, buffer, BUFFER_SIZE);
        char *sobrenome_atual = NULL;

        while(!achou && size_reg > 0){
            sobrenome_atual = strtok(buffer, PIPE);
            
            if(strcmp(sobrenome_atual, aux) == 0){
                achou = true;
            } else {
                size_reg = leia_registro(dados, buffer, BUFFER_SIZE);
            }
        }

        if(achou){
            Pessoa pe;
            parse(buffer, &pe);
            printf("Registro Encontrado:\n");
            printf("\tNome: %s\n", pe.nome);
            printf("\tSobrenome: %s\n", sobrenome_atual);
            printf("\tEndereço: %s\n", pe.endereco);
            printf("\tEstado: %s\n", pe.estado);
            printf("\tCidade: %s\n", pe.cidade);
            printf("\tCEP: %s\n", pe.cep);
        } else {
            printf("Registro não encontrado.\n");
        }

        return 0;
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

void parse(char *str, Pessoa *p){
    //continue com o ponteiro interno de strtok
    char *token = strtok(NULL, PIPE);
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