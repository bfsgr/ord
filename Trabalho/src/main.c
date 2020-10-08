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

    char buffer[BLOCO];

    while(le_bloco(operacoes, buffer) > 0){
        char *l = strtok(buffer, DELIM_REG);

        while(l != NULL){
            switch(l[0]){
                case 'b': 
                    busca(dados, l, true);
                    break;
                case 'r':
                    if( !remover(dados, l) )
                        printf("Erro: Registro não encontrado!\n\n");
                    break;
                case 'i':
                    inserir(dados, l);
                    break;
                default:
                    return false;
            }
            l = strtok(NULL, DELIM_REG);
        }
    }


    fclose(operacoes);
    fclose(dados);
    return true;
}

