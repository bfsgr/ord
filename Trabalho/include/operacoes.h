#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define BLOCO 4096 //Tamanho do bloco bufferizado do arquivo
#define REGISTRO 2048
#define DELIM_REG '\n'
#define DELIM_FIELD '|'
#define REMOVED '*'

bool importa(char *filename);
FILE* abrir_arquivo(char *filename, char* modo);
unsigned int le_bloco(FILE* arquivo, char* buffer);
bool escreve(FILE* arquivo, char *registro, short tamanho);
static bool cria_header(FILE* arquivo);

bool executa(char* arquivo);
int busca(FILE* arquivo, char* chave, bool printa);
short extrai_argumentos(char* operador);

bool remover(FILE* arquivo, char* chave);
static bool insere_led(FILE* arquivo, int offset);
bool inserir(FILE* arquivo, char* chave);
static void definir_op(FILE* arquivo, char* linha);

