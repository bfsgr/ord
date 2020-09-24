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
            printf("Ocorreu um erro durante a importação.\n");
        }
        
    } else if (strcmp(argv[1], "-e") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n\n", argv[2]);

        if(executa(argv[2])){
            printf("Execução finalizada com sucesso!\n");
        } else {
            printf("Ocorreu um erro durante a execução.\n");
        }

    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}

bool executa(char* arquivo){
    FILE* operacoes = abrir_arquivo(arquivo, "rb");
    //verifica se o arquivo de dados existe, se sim, abra com r+b
    FILE* dados = abrir_arquivo("dados.dat", "r");
    fclose(dados);
    dados = abrir_arquivo("dados.dat", "r+b");

    char buffer[BLOCO];

    while(le_bloco(operacoes, buffer) > 0){
        char *l = strtok(buffer, DELIM_REG);

        while(l != NULL){
            switch(l[0]){
                case 'b': 
                    busca(dados, l, true);
                    break;
                case 'r':
                    if( !remover(dados, l) )
                        printf("Erro: Registro não encontrado!\n\n");
                    break;
                case 'i':
                    inserir(dados, l);
                    break;
                default:
                    return false;
            }
            l = strtok(NULL, DELIM_REG);
        }
    }


    fclose(operacoes);
    fclose(dados);
    return true;
}

bool inserir(FILE* arquivo, char* chave){
    short tamanho = extrai_argumentos(chave);
    if(fseek(arquivo, 0, SEEK_SET) != 0){ return false; }
    
    int head = 0;
    if(fread(&head, sizeof(head), 1, arquivo) != 1) { return false; }

    bool fit = false;
    int last_head = 0;

    short size = 0;

    if(head == EOF){
        //escreva no final
        if(fseek(arquivo, 0, SEEK_END) != 0){ return false; }
        escreve(arquivo, chave);
        printf("Inserindo: \"%s\"\n", chave);
        printf("Inserido %i bytes no final\n\n", tamanho);
    } else {
        while(!fit && head != EOF){
            //vá para o offset de HEAD
            if(fseek(arquivo, head, SEEK_SET) != 0){ return false; }
            //leia o tamanho livre
            if(fread(&size, sizeof(size), 1, arquivo) != 1) { return false; }
            
            if(size >= tamanho){
                fit = true;
            } else {
                last_head = head;
                //pule o id "*|"
                if(fseek(arquivo, 2, SEEK_CUR) != 0){ return false; }
                if(fread(&head, sizeof(head), 1, arquivo) != 1) { return false; }
            }
        }

        if(fit){
            //pule o id "*|"
            if(fseek(arquivo, 2, SEEK_CUR) != 0){ return false; }
            int prox = 0;
            if(fread(&prox, sizeof(prox), 1, arquivo) != 1) { return false; }

            if(fseek(arquivo, last_head+4, SEEK_SET) != 0){ return false; }
            fwrite(&prox, sizeof(prox), 1, arquivo);

            if(fseek(arquivo, head, SEEK_SET) != 0){ return false; }
            escreve(arquivo, chave);
            printf("Inserindo: \"%s\"\n", chave);
            printf("Inserido %i bytes em %i\n", tamanho, head);
            printf("Sobra: %i bytes\n\n", size - tamanho);
        } else {
            if(fseek(arquivo, 0, SEEK_END) != 0){ return false; }
            escreve(arquivo, chave);
            printf("Inserindo: \"%s\"\n", chave);
            printf("Inserido %i bytes no final\n\n", tamanho);
        }
    }

    return true;
}

bool remover(FILE* arquivo, char* chave){
    int offset = busca(arquivo, chave, false);
    
    if(offset >= 0){
        if(fseek(arquivo, offset, SEEK_SET) != 0){ return false; }
        int nulo = -1; 
        short size = 0;
        if(fread(&size, sizeof(size), 1, arquivo) != 1) { return false; }

        if(fputc(REMOVED, arquivo) == EOF) { return false; };
        if(fputc(DELIM_FIELD[0], arquivo) == EOF) { return false; };
        if(fwrite(&nulo, sizeof(nulo), 1, arquivo) != 1) { return false; };

        if( !insere_led(arquivo, offset) ) { return false; }

        printf("Registro removido! (%i bytes)\n", size);
        printf("Offset: %i\n\n", offset);

        return true;
    } 

    return false;
}

bool insere_led(FILE* arquivo, int offset){
    int head = 0;

    if(fseek(arquivo, 0, SEEK_SET) != 0){ return false; }
    if(fread(&head, sizeof(head), 1, arquivo) != 1) { return false; }

    if(fseek(arquivo, 0, SEEK_SET) != 0){ return false; }
    if(fwrite(&offset, sizeof(offset), 1, arquivo) != 1) { return false; }

    if(head != -1){
        if(fseek(arquivo, offset+4, SEEK_SET) != 0){ return false; }
        if(fwrite(&head, sizeof(offset), 1, arquivo) != 1) { return false; }
    }

    return true;
}

int busca(FILE* arquivo, char* chave, bool printa){
    extrai_argumentos(chave);
    
    if(fseek(arquivo, sizeof(int), SEEK_SET) != 0){ 
        return false;
    }

    printa ? printf("Buscando registro de chave \"%s\"\n", chave) : false;

    int i = 2;
    char dados[BLOCO];
    unsigned int lidos = le_bloco(arquivo, dados);
    int blocos = 0;
    bool terminou = false;
    bool encontrado = false;
    int overhead = 0;
    unsigned short size = 0;

    while(lidos > 0 && !encontrado){
        
        while(!terminou){
            if(overhead > 0){
                if(overhead > 2){
                    unsigned char l = dados[overhead-2];
                    unsigned char h = dados[overhead-1];
                    size = l | h << 8;
                    i = overhead;
                } else {
                    size |= dados[0] << 8;
                    i = overhead;
                }
            } else {
                unsigned char l = dados[i-2];
                unsigned char h = dados[i-1];
                size = l | h << 8;
            } 
            
            int j = i;
            while(dados[j] !=  DELIM_FIELD[0]){
                j++;
            }
            dados[j] = '\0';

            if(strcmp(&dados[i], chave) == 0) {
                if((i + size) > lidos) {
                    printa ? printf("%s|", chave) : false;
                    printa ? printf("%s", &dados[j+1]) : false;
                    
                    char final[i + size - lidos];
                    if(fread(&final, 1, i + size - lidos, arquivo) != i + size - lidos) {
                        printf("Erro ao ler o arquivo.\n");
                        exit(1);
                    }
                    printa ? printf("%s (%i bytes)\n\n", final, size) : false;
                    terminou = true;
                    encontrado = true;

                } else {
                    printa ? printf("%s|", chave) : false;
                    dados[i + size] = '\0';
                    printa ? printf("%s (%i bytes)\n\n", &dados[j+1], size) : false;
                    terminou = true;
                    encontrado = true;
                }
            } else {
                if((i + size + 2) > lidos) {
                    terminou = true;
                    overhead = i + size + 2 - lidos;
                    if(overhead < 2){
                        unsigned char l = dados[lidos-1];
                        size = l;
                    }
                } else {
                    i = i + size + 2;
                    overhead = 0;
                }
            }
        }

        if(!encontrado){
            lidos = le_bloco(arquivo, dados);
            terminou = false;
            lidos == BLOCO-1 ? blocos++ : false;
        }

    }

    if(!encontrado){
        printa ? printf("Erro: registro não encontrado.\n\n") : false;
        return -1;
    } else {
        return (BLOCO-1) * blocos + i + 2;
    }

}

short extrai_argumentos(char* operador){
    unsigned int i = 2;
    while(operador[i] != '\0'){
        operador[i-2] = operador[i];
        i++;
    }
    operador[i-2] = '\0';
    return i-2;
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
    char regbuff[REGISTRO];
    regbuff[0] = '\0';
    char *p = NULL;
    bool fragmentado = false;

    while(le_bloco(dados, bloco) > 0){
        char *l = strtok_r(bloco, DELIM_REG, &p);
        while(l != NULL){
            if(*p != '\0'){
                if(fragmentado){
                    strcat(regbuff, l);
                    if(!escreve(dat, regbuff)){
                        return false;
                    }
                    regbuff[0] = '\0';
                    fragmentado = false;
                } else {
                    //registro completo no buffer, escreva-o
                    if(!escreve(dat, l)){
                        return false;
                    }
                }
            } else {
                //registro está fragmentado entre 2 ou mais blocos OU
                // esse é o último registro do arquivo OU
                // o buffer é pequeno d+ mas o buffer terminou mas o registro esta completo
                if(feof(dados) != 0) {
                    if(strlen(regbuff) > 0){
                        strcat(regbuff, l);
                        if(!escreve(dat, regbuff)){
                            return false;
                        }
                    } else {
                        if(!escreve(dat, l)){
                            return false;
                        }
                    }
                } else {
                    //salva a informação no buffer
                    strcat(regbuff, l);
                    fragmentado = true;
                }
            }
            l = strtok_r(NULL, DELIM_REG, &p);
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
//Encerra a execução em caso de erro de leitura
unsigned int le_bloco(FILE* arquivo, char* buffer){
    unsigned int lidos = fread(buffer, 1, BLOCO-1, arquivo);
    
    if(lidos < BLOCO-1){
        if(ferror(arquivo)){
            printf("Erro de leitura do arquivo\n");
            exit(1);
        } else {
            buffer[lidos] = '\0';
            return lidos;
        }
    } else {
        buffer[BLOCO-1] = '\0'; 
    }
    return lidos;
}

//Cria um header de 8 bytes para o manuseio da LED
//Retorna TRUE se o header for criado com sucesso
//Retorna FALSE se ocorrer um problema de escrita
bool cria_header(FILE* arquivo){
    int head = -1;
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
        printf("Um erro aconteceu ao abrir o arquivo %s.\n", filename);
        exit(1);
    } else {
        return fp;
    }
}