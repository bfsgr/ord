#include "../include/operacoes.h"

bool remover(FILE* arquivo, char* chave){
    int offset = busca(arquivo, chave, false);
    printf("Remoção do resgistro de chave \"%s\"\n", chave);
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

static bool insere_led(FILE* arquivo, int offset){
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

