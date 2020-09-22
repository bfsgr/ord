#include <stdio.h>
#include <stdbool.h>

#define BLOCO 4096 //Tamanho do bloco bufferizado do arquivo
#define REGISTRO 2048
#define DELIM_REG "\n" //
#define DELIM_FIELD "|" //

bool importa(char *filename);
FILE* abrir_arquivo(char *filename, char* modo);
unsigned int le_bloco(FILE* arquivo, char* buffer);
bool escreve(FILE* arquivo, char *registro);
bool cria_header(FILE* arquivo);

