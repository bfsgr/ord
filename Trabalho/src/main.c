#include "../include/operacoes.h"

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
    
    //cria um buffer de tamanho BLOCO
    char buffer[BLOCO];
    char opbuff[REGISTRO];
    char *prox = buffer;
    bool fragmentado = false;

    unsigned int lidos = le_bloco(operacoes, buffer);

    //enquanto for possivel ler blocos faça
    while(lidos > 0){
        char *teste = strchr(prox, DELIM_REG[0]);
        //quebre uma linha
        char *line = strtok_r(buffer, DELIM_REG, &prox);
        //enquanto houver linhas
        while(line != NULL){
            //se houver próximo
            if(*prox != '\0'){
                if(fragmentado){
                    //strtok não detecta delimitador no começo da string
                    //aqui esse caso é tratado
                    if(buffer[0] == '\n'){
                        definir_op(dados, opbuff);
                        opbuff[0] = '\0';
                        fragmentado = false;
                        definir_op(dados, line);
                    } else {
                        strcat(opbuff, line);
                        definir_op(dados, opbuff);
                        opbuff[0] = '\0';
                        fragmentado = false;
                    }
                } else {
                    definir_op(dados, line);
                }
            } else {
                //não há próximo
                //1. o arquivo acabou
                //2. o registro foi fragmentado entre os buffers
                //3. o registro coube perfeitamente no buffer e ele acabou
                if(feof(operacoes) != 0){
                    //sobrou algo no buffer de operações?
                    if(strlen(opbuff) > 0){
                        strcat(opbuff, line);
                        definir_op(dados, opbuff);
                    } else {
                        definir_op(dados, line);
                    }
                } else {
                    //3.
                    if(teste != NULL){
                        if(strlen(opbuff) > 0){
                            strcat(opbuff, line);
                            definir_op(dados, opbuff);
                        } else {
                            definir_op(dados, line);
                        }
                    } else {
                        //2.
                        //fragmentação entre buffers
                        strcat(opbuff, line);
                        fragmentado = true;
                    }
                }
            }
            teste = strchr(prox, DELIM_REG[0]);
            //leia a próxima linha
            line = strtok_r(NULL, DELIM_REG, &prox);
        }
        lidos = le_bloco(operacoes, buffer);
    }

    //feche os arquivos
    fclose(operacoes);
    fclose(dados);
    return true;
}

static void definir_op(FILE* arquivo, char* linha){
    //defina o caso da operação
    switch(linha[0]){
        case 'b': 
            busca(arquivo, linha, true);
            break;
        case 'r':
            if( !remover(arquivo, linha) )
                printf("Erro: Registro não encontrado!\n\n");
            break;
        case 'i':
            inserir(arquivo, linha);
            break;
        default:
            printf("Erro: Operação inválida!\n");
            exit(1);
            break;
    }
}
