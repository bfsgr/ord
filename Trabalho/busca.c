
#include "./operacoes.h"

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

