#include "../include/operacoes.h"

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
                    if(!escreve(dat, regbuff, strlen(regbuff))){
                        return false;
                    }
                    regbuff[0] = '\0';
                    fragmentado = false;
                } else {
                    //registro completo no buffer, escreva-o
                    if(!escreve(dat, l, strlen(l))){
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
                        if(!escreve(dat, regbuff, strlen(regbuff))){
                            return false;
                        }
                    } else {
                        if(!escreve(dat, l, strlen(l))){
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
bool escreve(FILE* arquivo, char *registro, short tamanho){
    if(fwrite(&tamanho, sizeof(tamanho), 1, arquivo) != 1){
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
static bool cria_header(FILE* arquivo){
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