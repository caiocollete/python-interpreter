#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insercao.h"
#include "variaveis.h"
#include "whileFunc.h"

void gerar_todas_as_linhas_print(No *lista, Variavel *listaVar);
Variavel procurar_variavel_print(Variavel *listaVar, char nome_variavel_a_ser_procurada[]);
Variavel encontrar_a_variavel_apos_o_percent(Token *linha, Variavel *listaVar, int posicao);
char *gerar_linha_print2(Token *linha, Variavel *listaVar);
int isInteger(const char *str);
int isFloat(const char *str);
int realizar_comparacao(tipo_atual tipo_operando1, Tipos valor_operando1, tipo_atual tipo_operando2, Tipos valor_operando2, char comparador[]);
char retornar_valor_de_apenas_uma_comparacao(Token *linha, Variavel *listaVar);
void testar_if();
char menu();

void executar_codigo_teste() {
    No *lista;
    Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
    char esc;

    inicializar(&lista);
    inserir_a_partir_de_arquivo(lista, "teste.py");

    // Processar a lista para identificar variaveis e funcoes
    processar_tokens(lista, &listaVar, &listaFunc);

    exibir_lista(lista);

    No *linha_atual = lista;
    while (linha_atual != NULL) {
        executar_linha_teste(&linha_atual, listaVar);
    }
}

void executar_linha_teste(No **linha_atual, Variavel *listaVar) {
    int eh_primeira_verificacao =
        1;  // Essa variável é para saber se entrou na função agora e, portanto,
            // fazer apenas uma verificação.
    // Por exemplo: print("a")
    // Só vai verificar uma vez qual o "comando" da linha, nesse caso é print,
    // mas poderia ser if, while etc.

    Token *token_atual = (*linha_atual)->tk;

    while (token_atual != NULL) {
        // Aqui verifica qual o comando da linha
        if (eh_primeira_verificacao) {
            if (strcmp(token_atual->token, "print") == 0) {
                printf("\n%s", gerar_linha_print2(token_atual, listaVar));
                eh_primeira_verificacao = 0;
            } else if (strcmp(token_atual->token, "if") == 0) {
                int resultado_comparacao =
                    retornar_valor_de_apenas_uma_comparacao(token_atual,
                                                            listaVar);
                *linha_atual = (*linha_atual)->prox;
                if (resultado_comparacao == 1) {
                    // Esse bloco vai executar os comandos que estiverem dentro
                    // do print

                    Token *token_aux = (*linha_atual)->tk;

                    // Esse while é necessário pois todos os comandos dentro do
                    // print possuem identação e, no nosso caso, a árvore de
                    // tokenização está contando \0 e espaços em branco, por
                    // isso foi necessário essas comparações
                    while (token_aux != NULL &&
                               strcmp(token_aux->token, "\t") == 0 ||
                           strcmp(token_aux->token, " ") == 0 ||
                           strcmp(token_aux->token, "\0") == 0) {
                        token_aux = token_aux->prox;
                    }
                    No *linha_aux = (No *)malloc(sizeof(No));
                    linha_aux->prox = NULL;
                    linha_aux->tk = token_aux;
                    executar_linha_teste(&linha_aux, listaVar);

                } else {
                    // Esse bloco é necessário caso não haja else após o if
                    while (*linha_atual != NULL &&
                               strcmp((*linha_atual)->tk->token, "\t") == 0 ||
                           strcmp((*linha_atual)->tk->token, " ") == 0 ||
                           strcmp((*linha_atual)->tk->token, "\0") == 0) {
                        *linha_atual = (*linha_atual)->prox;
                    }
                }

                if (resultado_comparacao == 1) {
                    // Esse bloco é necessário para pular o else caso a condição
                    // do if seja verdadeira
                    *linha_atual = (*linha_atual)->prox;
                    // printf("\nLinha atual = %s", (*linha_atual)->tk->token);
                    if (*linha_atual != NULL &&
                        strcmp((*linha_atual)->tk->token, "else") == 0) {
                        *linha_atual = (*linha_atual)->prox;
                        while (*linha_atual != NULL &&
                                   strcmp((*linha_atual)->tk->token, "\t") ==
                                       0 ||
                               strcmp((*linha_atual)->tk->token, " ") == 0 ||
                               strcmp((*linha_atual)->tk->token, "\0") == 0) {
                            *linha_atual = (*linha_atual)->prox;
                        }
                    }
                }
            }

        } else if (strcmp(token_atual->token, "else") == 0) {
            *linha_atual = (*linha_atual)->prox;

            // Esse bloco vai executar os comandos que estiverem dentro do print

            Token *token_aux = (*linha_atual)->tk;

            // Esse while é necessário pois todos os comandos dentro do print
            // possuem identação e, no nosso caso, a árvore de tokenização está
            // contando \0 e espaços em branco, por isso foi necessário essas
            // comparações
            while (token_aux != NULL && strcmp(token_aux->token, "\t") == 0 ||
                   strcmp(token_aux->token, " ") == 0 ||
                   strcmp(token_aux->token, "\0") == 0) {
                token_aux = token_aux->prox;
            }
            No *linha_aux = (No *)malloc(sizeof(No));
            linha_aux->prox = NULL;
            linha_aux->tk = token_aux;
            executar_linha_teste(&linha_aux, listaVar);
        }

        
        token_atual = token_atual->prox;
    }

    *linha_atual = (*linha_atual)->prox;
}

int main() {
    executar_codigo_teste();
    /*
    No *lista;
    Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
    char esc;

    inicializar(&lista);
    inserir_a_partir_de_arquivo(lista, "teste.py");

    //Processar a lista para identificar variaveis e funcoes
    processar_tokens(lista, &listaVar, &listaFunc);

    do{
            switch(menu()){
                    case 'b': 	exibir_variaveis(listaVar);
                                            printf("\n");
                                            exibir_funcoes(listaFunc);
                                            system("pause");
                                            break;

                    case 'a': 	executar_bloco(lista,listaVar,listaFunc);
                                            system("pause");
                                            break;

                    case 'c': 	exibir_lista(lista);
                                            system("pause");

            }
            scanf("%c",&esc);
            esc=tolower(esc);
    }while(esc!='s');


*/
}

char menu() {
    char esc;

    system("cls");

    printf("\n==== MENU ====\n");
    printf("[A] - Executar Codigo\n");
    printf("[B] - Exibir RAM\n");
    printf("[C] - Exibe Lista\n\n");

    printf("-> ");
    scanf("%c", &esc);

    system("cls");
    return tolower(esc);
}

// Função apenas para testar o if, será apgada depois

void testar_if() {
    No *lista;
    Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
    char esc;

    inicializar(&lista);
    inserir_a_partir_de_arquivo(lista, "teste.py");

    // Processar a lista para identificar variaveis e funcoes
    processar_tokens(lista, &listaVar, &listaFunc);
    /* MENU APAGADO POR ENQUANTO
    do{
            switch(menu()){
                    case 'b': 	exibir_variaveis(listaVar);
                                            printf("\n");
                                            exibir_funcoes(listaFunc);
                                            system("pause");
                                            break;

                    case 'a': 	executar_bloco(lista,listaVar,listaFunc);
                                            system("pause");
                                            break;

                    case 'c': 	exibir_lista(lista);
                                            system("pause");

            }
            scanf("%c",&esc);
            esc=tolower(esc);
    }while(esc!='s');

    */
    exibir_lista(lista);
    exibir_variaveis(listaVar);
    // gerar_todas_as_linhas_print(lista, listaVar);

    while (lista != NULL) {
        printf("\n%c",
               retornar_valor_de_apenas_uma_comparacao(lista->tk, listaVar));
        lista = lista->prox;
    }
}

Variavel procurar_variavel_print(Variavel *listaVar,
                                 char nome_variavel_a_ser_procurada[]) {
    Variavel aux;
    // J? ? atribu?do o nome de undefined para o caso de n?o encontrar a
    // vari?vel na lista de vari?veis
    strcpy(aux.nome, "undefined");
    // Verifica se a vari?vel a ser procurada est? na lista
    while (listaVar != NULL) {
        // Se encontrar a vari?vel, vai copiar para aux
        if (strcmp(nome_variavel_a_ser_procurada, listaVar->nome) == 0) {
            // Copia o nome
            strcpy(aux.nome, listaVar->nome);

            // Copia o valor e o tipo
            if (listaVar->tipoAtual == INT) {
                aux.valor.INT = listaVar->valor.INT;
                aux.tipoAtual = INT;
            } else if (listaVar->tipoAtual == FLOAT) {
                aux.valor.FLOAT = listaVar->valor.FLOAT;
                aux.tipoAtual = FLOAT;
            } else if (listaVar->tipoAtual == STRING) {
                strcpy(aux.valor.STR, listaVar->valor.STR);
                aux.tipoAtual = STRING;
            }
        }
        listaVar = listaVar->prox;
    }
    return aux;
}
Variavel encontrar_a_variavel_apos_o_percent(Token *linha, Variavel *listaVar,
                                             int posicao) {
    // Vai chegar no segundo "
    char
        nome_variavel_encontrada[10];  // ? o nome da vari?vel que est? ap?s o %
    char literal_string[40];  // Essa vari?vel ser? necess?ria quando, por
                              // exemplo, printf("%s", "7")
    Variavel var;             // ? a vari?vel que ser? retornada
    int encontrou_a_variavel = 0;
    while (linha != NULL && strcmp(linha->token, "\"") != 0)
        linha = linha->prox;
    if (linha != NULL) {
        linha = linha->prox;  // Pular o "

        // Agora precisa-se ultrapassar o % e o ( em print("..." % (...))
        while (linha != NULL && strcmp(linha->token, "(") != 0)
            linha = linha->prox;
        if (linha != NULL) linha = linha->prox;  // Estamos pulando o (

        // Agora precisa andar at? o argumento que est? em int posi??o
        int i = 0;
        while (i < posicao) {
            linha = linha->prox;
            if (strcmp(linha->token, ",") != 0 &&
                strcmp(linha->token, "\0") != 0 &&
                strcmp(linha->token, " ") != 0 && strcmp(linha->token, ")")) {
                // Esse if ? necess?rio para n?o contabilizar as  v?rgulas, \0 ,
                // espa?os em branco e )
                i++;
            }
        }
        if (strcmp(linha->token, "\"") == 0) {
            // Caso seja, por exemplo, printf("%s", "7")
            linha = linha->prox;
            strcpy(literal_string, linha->token);
            linha = linha->prox;
            while (strcmp(linha->token, "\"") != 0) {
                strcat(literal_string, " ");
                strcat(literal_string, linha->token);
                linha = linha->prox;
            }
            strcat(literal_string, "\0");
            strcpy(var.valor.STR, literal_string);
            var.tipoAtual = STRING;
            strcpy(var.nome, "literal");
        } else {
            strcpy(nome_variavel_encontrada, linha->token);
            var = procurar_variavel_print(listaVar, nome_variavel_encontrada);
        }
    }
    // printf("\nNome da variavel encontrada: %s", var.nome);
    return var;
}

char *gerar_linha_print2(Token *linha, Variavel *listaVar) {
    char print_gerado[MAX_LINHA];  // ? a string que ser? retornada
    strcpy(print_gerado, "");  // Retornar? uma string vazia caso aux == NULL ou
                               // n?o seja um print
    Token *aux = linha;
    Token *aux2;  // ? a vari?vel que ir? procurar os argumentos ap?s o % em
                  // print("..." % (...))
    int contador_posicao = 0;  // Ir? contar quantos %d %s %f tem, para saber
                               // qual deve ser procurado ap?s o %

    // Esse primeiro if ir? verificar se a vari?vel que guarda os tokens n?o ?
    // NULL e se realmente ? uma linha que tem print
    if (aux != NULL && strcmp(aux->token, "print") == 0) {
        // Todos os prints tem essa estrutura print("..." % )
        // Caso entre aqui, a condi??o de parada ser? chegar no segundo "
        // Para isso, precisa-se pular o ( e " primeiro, porque sen?o ir? parar
        // nesse primeiro "
        while (strcmp(aux->token, "\"") != 0) aux = aux->prox;
        aux = aux->prox;  // O token que estava armazenado ao sair do while era
                          // o ", ent?o j? pulamos um pra frente

        // A condi??o de parada desse while ser? o pr?ximo " , pois o que vier
        // depois dele s? ? necess?rio para descobrir quais os valores dos %d %s
        // %f caso tenham
        while (strcmp(aux->token, "\"") != 0) {
            // Aqui ? onde ser? tratado caso encontre %d %s %f
            if (strcmp(aux->token, "%d") == 0 ||
                strcmp(aux->token, "%s") == 0 ||
                strcmp(aux->token, "%f") == 0) {
                aux2 = aux;
                Variavel var_encontrada = encontrar_a_variavel_apos_o_percent(
                    aux, listaVar, contador_posicao);
                char tipo_para_string[10];
                if (strcmp(var_encontrada.nome, "undefined") != 0) {
                    if (var_encontrada.tipoAtual == INT) {
                        sprintf(tipo_para_string, "%d",
                                var_encontrada.valor.INT);
                        strcat(print_gerado, tipo_para_string);
                        strcat(print_gerado, " ");
                    } else if (var_encontrada.tipoAtual == FLOAT) {
                        sprintf(tipo_para_string, "%f",
                                var_encontrada.valor.FLOAT);

                        strcat(print_gerado, tipo_para_string);
                        strcat(print_gerado, " ");
                    } else {
                        strcat(print_gerado, var_encontrada.valor.STR);
                        strcat(print_gerado, " ");
                    }
                } else {
                    strcat(print_gerado, var_encontrada.nome);
                    strcat(print_gerado, " ");
                }
                contador_posicao++;

            } else {
                // Aqui ? onde concatena na string a ser gerada caso n?o seja %d
                // %s e %f
                strcat(print_gerado, aux->token);
                strcat(print_gerado, " ");
            }
            // printf("%s", aux->token);
            aux = aux->prox;
        }
    }
    return print_gerado;
}

int isInteger(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

int isFloat(const char *str) {
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}

int realizar_comparacao(tipo_atual tipo_operando1, Tipos valor_operando1,
                        tipo_atual tipo_operando2, Tipos valor_operando2,
                        char comparador[]) {
    char comparador_igualdade[3],   // ==
        comparador_diferenca[3],    // !=
        comparador_maior[2],        // >
        comparador_menor[2],        // <
        comparador_maior_igual[3],  // >=
        comparador_menor_igual[3];  // <=

    strcpy(comparador_igualdade, "==");
    strcpy(comparador_diferenca, "!=");
    strcpy(comparador_maior, ">");
    strcpy(comparador_menor, "<");
    strcpy(comparador_maior_igual, ">=");
    strcpy(comparador_menor_igual, "<=");

    // Faz as comparacoes caso ambos sejam inteiros
    if (tipo_operando1 == INT && tipo_operando2 == INT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.INT == valor_operando2.INT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.INT != valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.INT > valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.INT < valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.INT >= valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.INT <= valor_operando2.INT);

    } else if (tipo_operando1 == FLOAT && tipo_operando2 == FLOAT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.FLOAT == valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.FLOAT != valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.FLOAT > valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.FLOAT < valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.FLOAT >= valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.FLOAT <= valor_operando2.FLOAT);
    } else if (tipo_operando1 == INT && tipo_operando2 == FLOAT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.INT == valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.INT != valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.INT > valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.INT < valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.INT >= valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.INT <= valor_operando2.FLOAT);
    } else if (tipo_operando1 == FLOAT && tipo_operando2 == INT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.FLOAT == valor_operando2.INT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.FLOAT != valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.FLOAT > valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.FLOAT < valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.FLOAT >= valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.FLOAT <= valor_operando2.INT);
    } else if (tipo_operando1 == STRING && tipo_operando2 == STRING) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return !strcmp(valor_operando1.STR, valor_operando2.STR);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return !strcmp(valor_operando1.STR, valor_operando2.STR);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) > 0);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) < 0);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) >= 0);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) <= 0);
    }

    return -1;  // Esse valor ? retornado caso n?o seja poss?vel concluir se uma
                // express?o ? verdadeira ou falsa
}

char retornar_valor_de_apenas_uma_comparacao(Token *linha, Variavel *listaVar) {
    // Essa fun??o ? respons?vel por separar os operadores de uma compara??o,
    // por exemplo: 1 == 4 Ela ir? separar os tokens [1, ==, 4] e, ap?s
    // separ?-los, ir? jog?-los em uma fun??o que retorna o valor dessa
    // compara??o E a? a fun??o atual ir? retornar o valor

    Tipos valor_do_operando1;
    Tipos valor_do_operando2;
    tipo_atual tipo_do_operando1;
    tipo_atual tipo_do_operando2;
    char comparador_utilizado[3];
    strcpy(comparador_utilizado, "");

    // Verifica se a linha n?o ? NULL e se realmente ? um if ou elif
    // printf("\neh um if");
    if (linha != NULL && (strcmp(linha->token, "if") == 0 ||
                          strcmp(linha->token, "elif") == 0)) {
        linha = linha->prox;

        // Vai chegar no primeiro token da compara??o
        // Esse while ? necess?rio caso haja '\0' ou espa?os em branco na linha
        while (linha != NULL && strcmp(linha->token, " ") == 0 ||
               strcmp(linha->token, "\0") == 0) {
            linha = linha->prox;
        }

        // Esse bloco de c?digo ? respons?vel por pegar o primeiro operando (o
        // operando que est? ? esquerda do operador, ex: 1 == 2     Esse bloco
        // de c?digo ir? pegar o 1 Aqui ir? pegar o primeiro operando Verifica
        // se ? uma string
        if (strcmp(linha->token, "\"") == 0) {
            // Se for uma string, ir? armazen?-la

            tipo_do_operando1 = STRING;
            char string_a_armazenar[MAX_TOKEN];
            strcpy(string_a_armazenar, "");
            linha = linha->prox;
            while (strcmp(linha->token, "\"") != 0) {
                strcat(string_a_armazenar, linha->token);
                strcat(string_a_armazenar, " ");
                linha = linha->prox;
            }
            strcpy(valor_do_operando1.STR, string_a_armazenar);
            // printf("Operando1 = %s", valor_do_operando1.STR);
            linha = linha->prox;
        } else {
            // Se n?o for string, s? pode ser um n?mero ou uma vari?vel
            // Se for para o else, ent?o ? float, porque n?o ? inteiro e nem
            // string
            if (isInteger(linha->token)) {
                tipo_do_operando1 = INT;
                valor_do_operando1.INT = atoi(linha->token);
                // printf("\nOperando1 = %d", valor_do_operando1.INT);
            } else if (isFloat(linha->token)) {
                tipo_do_operando1 = FLOAT;
                valor_do_operando1.FLOAT = atof(linha->token);
                // printf("\nOperando1 = %f", valor_do_operando1.FLOAT);
            } else {
                Variavel var = procurar_variavel_print(listaVar, linha->token);
                tipo_do_operando1 = var.tipoAtual;
                if (tipo_do_operando1 == FLOAT) {
                    valor_do_operando1.FLOAT = var.valor.FLOAT;
                    // printf("\nOperando1 = %f", valor_do_operando1.FLOAT);
                } else if (tipo_do_operando1 == INT) {
                    valor_do_operando1.INT = var.valor.INT;
                    // printf("\nOperando1 = %d", valor_do_operando1.INT);
                } else {
                    strcpy(valor_do_operando1.STR, var.valor.STR);
                    // printf("\nOperando1 = %s", valor_do_operando1.STR);
                }
            }
        }

        linha = linha->prox;
        // Esse bloco de c?digo ir? pegar o operador, em 1 == 2 vai pegar ==
        // Esse while ? necess?rio caso haja '\0' ou espa?os em branco na linha
        // ap?s o primeiro operando
        while (linha != NULL && strcmp(linha->token, " ") == 0 ||
               strcmp(linha->token, "\0") == 0) {
            linha = linha->prox;
        }

        // Esse bloco de c?digo ir? armazenar o comparador
        // Como nossa lista est? armazenado '\0' e espa?os em branco, ?
        // necess?rio verificar um por um sen?o o c?digo n?o roda O token ==
        // est? sendo armazenado como = -> -> = , ent?o ele ser? verificado
        // primeiro
        // Se entrar no if, significa que ? o == e ser? preciso armazen?-lo
        if (strcmp(linha->token, "=") == 0) {
            strcat(comparador_utilizado, linha->token);
            while (linha != NULL && strcmp(linha->token, "=") != 0) {
                linha = linha->prox;
            }
            strcat(comparador_utilizado, linha->token);
            linha = linha->prox;
            linha = linha->prox;
        } else if ((strcmp(linha->token, ">") == 0 ||
                    strcmp(linha->token, "<") == 0) &&
                   (strcmp(linha->prox->token, "=") != 0 &&
                    strcmp(linha->prox->prox->token, "=") != 0)) {
            // O comparador < e > n?o tem '\0' nem espa?o em branco ap?s
            strcat(comparador_utilizado, linha->token);
        } else {
            // Os comparadores restantes agem da mesma forma, tem '\0' ou espa?o
            // em branco ap?s eles
            strcat(comparador_utilizado, linha->token);
            while (linha != NULL && strcmp(linha->token, "=") != 0) {
                linha = linha->prox;
            }
            strcat(comparador_utilizado, linha->token);
        }

        // printf("\nComparador %s", comparador_utilizado);

        // Esse pr?ximo bloco de c?digo ? respons?vel por pegar o segundo
        // operando
        linha = linha->prox;
        // Esse while ? necess?rio caso haja '\0' ou espa?os em branco na linha
        // ap?s o primeiro operando
        while (linha != NULL && (strcmp(linha->token, " ") == 0 ||
                                 strcmp(linha->token, "\0") == 0)) {
            linha = linha->prox;
        }

        if (strcmp(linha->token, "\"") == 0) {
            // Se for uma string, ir? armazen?-la

            tipo_do_operando2 = STRING;
            char string_a_armazenar[MAX_TOKEN];
            strcpy(string_a_armazenar, "");
            linha = linha->prox;
            while (strcmp(linha->token, "\"") != 0) {
                strcat(string_a_armazenar, linha->token);
                strcat(string_a_armazenar, " ");
                linha = linha->prox;
            }
            strcpy(valor_do_operando2.STR, string_a_armazenar);
            // printf("\nOperando2 = %s", valor_do_operando2.STR);
        } else {
            // Se n?o for string, s? pode ser um n?mero e a? ? necess?rio
            // descobrir se ? inteiro ou float Se for para o else, ent?o ?
            // float, porque n?o ? inteiro e nem string
            if (isInteger(linha->token)) {
                tipo_do_operando2 = INT;
                valor_do_operando2.INT = atoi(linha->token);
                // printf("\nOperando2 = %d", valor_do_operando2.INT);
            } else if (isFloat(linha->token)) {
                tipo_do_operando2 = FLOAT;
                valor_do_operando2.FLOAT = atof(linha->token);
                // printf("\nOperando2 = %f", valor_do_operando2.FLOAT);
            } else {
                Variavel var = procurar_variavel_print(listaVar, linha->token);
                tipo_do_operando2 = var.tipoAtual;
                if (tipo_do_operando2 == FLOAT) {
                    valor_do_operando2.FLOAT = var.valor.FLOAT;
                    // printf("\nOperando2 = %f", valor_do_operando2.FLOAT);
                } else if (tipo_do_operando2 == INT) {
                    valor_do_operando2.INT = var.valor.INT;
                    // printf("\nOperando2 = %d", valor_do_operando2.INT);
                } else {
                    // printf("\nEntrou3");
                    strcpy(valor_do_operando2.STR, var.valor.STR);
                    // printf("\nOperando2 = %s", valor_do_operando2.STR);
                }
            }
        }
        // printf("\n Resultado da operacao: %d",
        // realizar_comparacao(tipo_do_operando1, valor_do_operando1,
        // tipo_do_operando2, valor_do_operando2, comparador_utilizado));
    }
    return realizar_comparacao(tipo_do_operando1, valor_do_operando1,
                               tipo_do_operando2, valor_do_operando2,
                               comparador_utilizado);
}

