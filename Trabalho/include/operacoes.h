#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BLOCO 4096 //Tamanho do bloco bufferizado do arquivo
#define REGISTRO 2048
#define DELIM_REG '\n'
#define DELIM_FIELD '|'
#define REMOVED '*'

void importa(char *filename);
FILE* abrir_arquivo(char *filename, char* modo);
unsigned int le_bloco(FILE* arquivo, char* buffer);
void escreve(FILE* arquivo, char *registro, short tamanho);

void executa(char* arquivo);
int busca(FILE* arquivo, char* chave, bool printa);
short extrai_argumentos(char* operador);

bool remover(FILE* arquivo, char* chave);
void inserir(FILE* arquivo, char* chave);