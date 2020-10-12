#include "../include/operacoes.h"

static void definir_op(FILE* arquivo, char* linha);

int main(int argc, char **argv) {

    if (argc < 3) {
        fprintf(stderr, "Numero incorreto de argumentos!\n");
        fprintf(stderr, "Modo de uso:\n");
        fprintf(stderr, "$ %s (-i|-e) nome_arquivo\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "-i") == 0) {
        printf("Modo de importacao ativado ... nome do arquivo = %s\n", argv[2]);
        
        importa(argv[2]);
        printf("Importação realizada com sucesso!\n");
        
    } else if (strcmp(argv[1], "-e") == 0) {
        printf("Modo de execucao de operacoes ativado ... nome do arquivo = %s\n\n", argv[2]);
        //chama a execução do arquivo de operações
        executa(argv[2]);
        printf("Execução finalizada com sucesso!\n");

    } else {
        fprintf(stderr, "Opcao \"%s\" nao suportada!\n", argv[1]);
    }

    return 0;
}

void executa(char* arquivo){
    FILE* operacoes = abrir_arquivo(arquivo, "r");
    //verifica se o arquivo de dados existe, se sim, abra com r+b
    FILE* dados = abrir_arquivo("dados.dat", "r");
    fclose(dados);
    dados = abrir_arquivo("dados.dat", "r+b");
    
    char bloco[BLOCO];
    char regbuff[REGISTRO];
    regbuff[0] = '\0';
    bool fragmentado = false;
    int iter = 0;
    
    unsigned int lidos = le_bloco(operacoes, bloco);

    while(lidos > 0){
        while(iter < lidos){
            //salve o começo do registro atual
            int start = iter;
            //itere até encontrar um \n (Delimitador de registro)
            while(iter < lidos && bloco[iter] != DELIM_REG){
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
                        definir_op(dados, regbuff);
                        regbuff[0] = '\0';
                        fragmentado = false;
                    } else {
                        definir_op(dados, &bloco[start]);
                    }
                    iter++;
                } else {
                    //o registro está fragmentado entre blocos ou é o fim do arquivo
                    if(feof(dados) != 0){
                        if(strlen(regbuff) > 0){
                            strcat(regbuff, &bloco[start]);
                            definir_op(dados, regbuff);
                        }else {
                            definir_op(dados, &bloco[start]);
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
                    definir_op(dados, regbuff);
                    regbuff[0] = '\0';
                    fragmentado = false;
                } else {
                     definir_op(dados, &bloco[start]);
                }
                iter++;
            }
        }
        lidos = le_bloco(operacoes, bloco);
        iter = 0;
    }
    //feche os arquivos
    fclose(operacoes);
    fclose(dados);
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
