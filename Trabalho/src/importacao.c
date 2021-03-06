#include "../include/operacoes.h"

static void cria_header(FILE* arquivo);

//Importa os dados de um arquivo arbitrário e os escreve em dados.dat
void importa(char* filename){
    FILE* dados = abrir_arquivo(filename, "r");
    FILE* dat = abrir_arquivo("dados.dat", "w");

    cria_header(dat);
    
    //Buffer do bloco e de registro
    char bloco[BLOCO];
    char regbuff[REGISTRO];
    //Zera o buffer de registro 
    regbuff[0] = '\0';
    //flag de controle
    bool fragmentado = false;
    //iterador
    int iter = 0;
    
    unsigned int lidos = le_bloco(dados, bloco);

    while(lidos > 0){
        while(iter < lidos){
            //salve o começo do registro atual
            int start = iter;
            //itere até encontrar um \n (Delimitador de registro)
            while(iter < lidos && bloco[iter] != DELIM_REG ){
                iter++;
            }

            //estamos no final do bloco?
            if(iter == lidos){
                //o registro está completo?
                if(bloco[iter] == DELIM_REG){
                    //marque o final e escreva o registro no disco
                    bloco[iter] = '\0';
                    //A flag de fragmentação está ligada?
                    if(fragmentado){
                        strcat(regbuff, &bloco[start]);
                        escreve(dat, regbuff, strlen(regbuff));
                        regbuff[0] = '\0';
                        fragmentado = false;
                    } else {
                        escreve(dat, &bloco[start], iter - start);
                    }
                    iter++;
                } else {
                    //o registro está fragmentado entre blocos ou é o fim do arquivo
                    if(feof(dados) != 0){
                        if(strlen(regbuff) > 0){
                            strcat(regbuff, &bloco[start]);
                            escreve(dat, regbuff, strlen(regbuff));
                        }else {
                            escreve(dat, &bloco[start], iter - start);
                        }
                    } else {
                        fragmentado = true;
                        strcat(regbuff, &bloco[start]);
                    }
                }
            } else {
                //o registro está interiamente disponível na memória, escrevá-o
                bloco[iter] = '\0';
                //A flag de fragmentação está ligada?
                if(fragmentado){
                    strcat(regbuff, &bloco[start]);
                    escreve(dat, regbuff, strlen(regbuff));
                    regbuff[0] = '\0';
                    fragmentado = false;
                } else {
                    escreve(dat, &bloco[start], iter - start);
                }
                iter++;
            }
        }
        lidos = le_bloco(dados, bloco);
        iter = 0;
    }

    fclose(dat);
    fclose(dados);
}


//Escreve um registro arbitrário na cabeça de leitura atual do arquivo de dados
//Prefixa o registro com o seu tamanho
void escreve(FILE* arquivo, char *registro, short tamanho){
    fwrite(&tamanho, sizeof(tamanho), 1, arquivo);
    fputs(registro, arquivo);
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
static void cria_header(FILE* arquivo){
    int head = -1;
    fwrite(&head, sizeof(head), 1, arquivo);
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