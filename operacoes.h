#include <stdio.h>
#include <stdbool.h>

#define BLOCO 1024 //Tamanho do bloco bufferizado do arquivo
#define BUFF_REG 500 //Tamanho máximo de um registro na memória

bool importa(char *filename);
FILE* abrir_arquivo(char *filename, char* modo);
bool le_bloco(FILE* arquivo, char* buffer);
bool ler_registro(char* bloco, char* registro);
bool escreve(FILE* arquivo, char *registro);
short tamanho(char *registro);
