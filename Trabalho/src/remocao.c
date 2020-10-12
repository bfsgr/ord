#include "../include/operacoes.h"

static void insere_led(FILE* arquivo, int offset);

bool remover(FILE* arquivo, char* chave){
    int offset = busca(arquivo, chave, false);
    printf("Remoção do resgistro de chave \"%s\"\n", chave);
    if(offset >= 0){
        fseek(arquivo, offset, SEEK_SET);
        int nulo = -1; 
        short size = 0;
        fread(&size, sizeof(size), 1, arquivo);

        fputc(REMOVED, arquivo);
        fputc(DELIM_FIELD, arquivo);
        fwrite(&nulo, sizeof(nulo), 1, arquivo);

        insere_led(arquivo, offset);

        printf("Registro removido! (%i bytes)\n", size);
        printf("Offset: %i\n\n", offset);

        return true;
    } 

    return false;
}

static void insere_led(FILE* arquivo, int offset){
    int head = 0;

    fseek(arquivo, 0, SEEK_SET);
    fread(&head, sizeof(head), 1, arquivo);

    fseek(arquivo, 0, SEEK_SET);
    fwrite(&offset, sizeof(offset), 1, arquivo);

    if(head != -1){
        fseek(arquivo, offset+4, SEEK_SET);
        fwrite(&head, sizeof(offset), 1, arquivo);
    }
}

