#include <stdio.h>
#include <stdbool.h>

#define BLOCO 4096 //Tamanho do bloco bufferizado do arquivo, buffers pequenos causam problemas
#define REGISTRO 1024
#define DELIM_LINE "\n" //
#define DELIM_FIELD "|" //

bool importa(char *filename);
FILE* abrir_arquivo(char *filename, char* modo);
unsigned int le_bloco(FILE* arquivo, char* buffer);
bool escreve(FILE* arquivo, char *registro);
bool cria_header(FILE* arquivo);

bool executa(char* arquivo);
void busca(FILE* arquivo, char* operador);
void extrai_argumentos(char* operador);