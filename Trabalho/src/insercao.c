#include "../include/operacoes.h"

void inserir(FILE* arquivo, char* chave){
    short tamanho = extrai_argumentos(chave);
    fseek(arquivo, 0, SEEK_SET);
    
    int head = 0;
    fread(&head, sizeof(head), 1, arquivo);

    bool fit = false;
    int last_head = 0;

    short size = 0;

    if(head == EOF){
        //escreva no final
        fseek(arquivo, 0, SEEK_END);
        escreve(arquivo, chave, tamanho);
        printf("Inserindo: \"%s\"\n", chave);
        printf("Inserido %i bytes no final\n\n", tamanho);
    } else {
        while(!fit && head != EOF){
            //vá para o offset de HEAD
            fseek(arquivo, head, SEEK_SET);
            //leia o tamanho livre
            fread(&size, sizeof(size), 1, arquivo);
            
            if(size >= tamanho+2){
                fit = true;
            } else {
                last_head = head;
                //pule o id "*|"
                fseek(arquivo, 2, SEEK_CUR);
                fread(&head, sizeof(head), 1, arquivo);
            }
        }

        if(fit){
            short sobra = size - tamanho - 2;
            printf("Inserindo: \"%s\"\n", chave);
            printf("Local: offset %i\n", head);
            printf("Tamanho livre: %i bytes\n", size);
            printf("Tamanho da sobra: %i bytes\n", sobra);
            if(sobra >= 50){
                fseek(arquivo, -2 , SEEK_CUR);
                fwrite(&sobra, sizeof(short), 1, arquivo);
                fseek(arquivo, head + sobra + 2, SEEK_SET);
                escreve(arquivo, chave, tamanho);
                printf("Inserido %i bytes em %i (%i+%i)\n\n", tamanho, head+sobra, head, sobra);
            } else {
                //pule o id "*|"
                fseek(arquivo, 2, SEEK_CUR);
                int prox = 0;
                fread(&prox, sizeof(prox), 1, arquivo);

                if(last_head == 0){
                    fseek(arquivo, 0, SEEK_SET);
                    fwrite(&prox, sizeof(prox), 1, arquivo);
                } else {
                    fseek(arquivo, last_head+4, SEEK_SET);
                    fwrite(&prox, sizeof(prox), 1, arquivo);
                }

                fseek(arquivo, head, SEEK_SET);
            
                escreve(arquivo, chave, size);
                if(tamanho < size) { fputc('\0', arquivo); }

                printf("Inserido %i bytes em %i (0x%x)\n\n", tamanho, head, head);
            }
        } else {
            fseek(arquivo, 0, SEEK_END);
            escreve(arquivo, chave, tamanho);
            printf("Inserindo: \"%s\"\n", chave);
            printf("Inserido %i bytes no final\n\n", tamanho);
        }
    }
}

