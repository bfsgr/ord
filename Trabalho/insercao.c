#include "./operacoes.h"

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
        escreve(arquivo, chave, tamanho);
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

            if(last_head == 0){
                if(fseek(arquivo, 0, SEEK_SET) != 0){ return false; }
                fwrite(&prox, sizeof(prox), 1, arquivo);
            } else {
                if(fseek(arquivo, last_head+4, SEEK_SET) != 0){ return false; }
                fwrite(&prox, sizeof(prox), 1, arquivo);
            }

            if(fseek(arquivo, head, SEEK_SET) != 0){ return false; }

            escreve(arquivo, chave, size);
            if(tamanho < size) { fputc('\0', arquivo); }


            printf("Inserindo: \"%s\"\n", chave);
            printf("Inserido %i bytes em %i\n", tamanho, head);
            printf("Sobra: %i bytes\n\n", size - tamanho - 2);
        } else {
            if(fseek(arquivo, 0, SEEK_END) != 0){ return false; }
            escreve(arquivo, chave, tamanho);
            printf("Inserindo: \"%s\"\n", chave);
            printf("Inserido %i bytes no final\n\n", tamanho);
        }
    }

    return true;
}

