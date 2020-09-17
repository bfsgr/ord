#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int leia_campo(FILE *fp, char *str, int tam);
FILE* open_file(char *str);
int input(char *str, int size);

int main(){
    char filename[50];
    printf("Digite o nome do arquivo a ser aberto: ");
    input(filename, 50);
    FILE *entrada = open_file(filename);

    char campo[255];

    int lenght = leia_campo(entrada, campo, 255);
    int count = 0;

    while(lenght > 0){
        printf("campo %i: %s\n", count++, campo);
        lenght = leia_campo(entrada, campo, 255);
    }

    fclose(entrada);
    return 0;
}

int leia_campo(FILE *fp, char *str, int tam){
    int i = 0;
    char read = fgetc(fp);

    while( read != EOF && read != '|'){
        if(tam > 1){
            str[i] = read;
            i++;
            tam--;
        }
        read = fgetc(fp);
    }
    str[i] = '\0';
    return i;
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