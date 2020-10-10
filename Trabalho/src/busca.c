
#include "../include/operacoes.h"

int busca(FILE* arquivo, char* chave, bool printa){
    //remove os dois primeiros bytes da string chave
    extrai_argumentos(chave);
    //vá para o começo do arquivo (depois do cabeçalho)
    if(fseek(arquivo, sizeof(int), SEEK_SET) != 0){ 
        return false;
    }
    //se printa então mostre as informações na tela
    printa ? printf("Buscando registro de chave \"%s\"\n", chave) : false;
    //i = o primeiro byte do registro sem o tamanho
    int i = 2;
    //cria um buffer de tamanho BLOCO
    char dados[BLOCO];
    //o número de bytes lidos por le_bloco
    unsigned int lidos = le_bloco(arquivo, dados);
    //a contagem de blocos lidos até o momento
    int blocos = 0;
    //booleanos de controle
    bool terminou = false;
    bool encontrado = false;
    //auxiliar para a fragmentação entre buffers
    int overhead = 0;
    //tamanho do registro lido no momento
    unsigned short size = 0;

    //se existirem bytes a serem lidos e não encontramos a chave
    while(lidos > 0 && !encontrado){
        //enquanto o buffer não terminar de ser processado
        while(!terminou){
            //há fragmentação entre buffers?
            if(overhead > 0){
                //trata o caso patológico da fragmentação.
                if(overhead > 2){
                    //le o tamanho com base no número de bytes que ultrapassaram o buffer
                    size = (unsigned char) dados[overhead-2] | (unsigned char) dados[overhead-1] << 8;
                    i = overhead;
                } else {
                    //se o tamanho se perdeu entre dois buffers
                    size |= (unsigned char) dados[0] << 8;
                    i = overhead;
                }
            } else {
                //não houve fragmentação
                size = (unsigned char) dados[i-2] | (unsigned char) dados[i-1] << 8;
            } 
            
            //aumente j até encontrar um | (DELIM_FIELD)
            int j = i;
            while(dados[j] !=  DELIM_FIELD[0]){
                j++;
            }
            //o substitua por \0
            dados[j] = '\0';

            //compare a string iniciada no endereço de dados[i] com a chave buscada
            if(strcmp(&dados[i], chave) == 0) {
                //o registro está inteiramente no buffer ou não?
                if((i + size) > lidos) {
                    //printe a chave do registro
                    printa ? printf("%s|", chave) : false;
                    //printe a string iniciada após a chave
                    printa ? printf("%s", &dados[j+1]) : false;
                    //crie um mini-buffer para ler a parte faltante do registro
                    char final[i + size - lidos];
                    //leia os bytes faltantes
                    if(fread(&final, 1, i + size - lidos, arquivo) != i + size - lidos) {
                        printf("Erro ao ler o arquivo.\n");
                        exit(1);
                    }
                    //printe a parte lida
                    printa ? printf("%s (%i bytes)\n\n", final, size) : false;
                    //termine a operação no buffer e sete encontrado
                    terminou = true;
                    encontrado = true;

                } else {
                    //printe a chave
                    printa ? printf("%s|", chave) : false;
                    //marque o final do registro
                    dados[i + size] = '\0';
                    //printe o registro sem a chave
                    printa ? printf("%s (%i bytes)\n\n", &dados[j+1], size) : false;
                    //termine a operação no buffer e sete encontrado
                    terminou = true;
                    encontrado = true;
                }
            } else {
                //o registro estoura o buffer?
                if((i + size + 2) > lidos) {
                    //termine a operação no buffer
                    terminou = true;
                    //defina os bytes a serem pulados no próximo bloco
                    overhead = i + size + 2 - lidos;
                    //caso apenas um bytes seja passado, temos o caso patológico onde o tamanho está quebrado entre buffers
                    if(overhead < 2){
                        size = (unsigned char) dados[lidos-1];;
                    }
                } else {
                    //não estoura o buffer, pule o tamanho do registro + 2
                    i = i + size + 2;
                    //bytes ultrapassados são 0
                    overhead = 0;
                }
            }
        }
        //se não foi encontrado faça
        if(!encontrado){
            //leia o próximo bloco
            lidos = le_bloco(arquivo, dados);
            //defina terminou como false
            terminou = false;
            //se o tamanho lido for um bloco completo, incremente blocos em 1
            lidos == BLOCO-1 ? blocos++ : false;
        }

    }

    //se não foi encontrado
    if(!encontrado){
        //printe um erro e retorne o offset como -1
        printa ? printf("Erro: registro não encontrado.\n\n") : false;
        return -1;
    } else {
        //o registro foi encontrado, calcule o offset 
        return (BLOCO-1) * blocos + i + 2;
    }

}

short extrai_argumentos(char* operador){
    //desloque a string dois bytes para a esquerda, retorne o tamanho
    unsigned int i = 2;
    while(operador[i] != '\0'){
        operador[i-2] = operador[i];
        i++;
    }
    operador[i-2] = '\0';
    return i-2;
}

