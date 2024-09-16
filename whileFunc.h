#include "listaGen.h"
void executar_linha(Token *linha, Variavel *listaVar, No *bloco, Funcao *listaFunc) {
    while (linha != NULL) {
        if (strcmp(linha->token, "while") == 0) {
            executar_while(linha, listaVar, bloco, listaFunc);
            
        } 
		else if (strcmp(linha->token, "if") == 0) {
            // Implementar l�gica para if
        } 
		else if (strcmp(linha->token, "elif") == 0) {
            // Implementar l�gica para elif
        } 
		else if (strcmp(linha->token, "else") == 0) {
            // Implementar l�gica para else
        } 
		else if (strcmp(linha->token, "for") == 0) {
            // Implementar l�gica para for
        } 
		else if (!ehDelimitador(linha->token[0])) {
            // Verificar se o token � uma vari�vel
            Variavel *var = procurar_variavel(listaVar, linha->token);
            if (var != NULL && strcmp(var->nome, "undefined") != 0) {
                linha = linha->prox; // mover para o pr�ximo token (poss�vel operador de atribui��o)
                if (linha != NULL && strcmp(linha->token, "=") == 0) {
                    linha = linha->prox; // mover para o valor a ser atribu�do
                    if (linha != NULL) {
                        // Parse para ListaGen e resolver a express�o
                        ListaGen *listaGen = ParseListaEncadeada(linha,listaVar);
                        var->valor.FLOAT = ResolveLista(listaGen);
                    }
                }
            } else {
                printf("Comando ou vari�vel '%s' n�o reconhecido(a).\n", linha->token);
            }
        }
        linha = linha->prox; // mover para a pr�xima linha
    }
}

void executar_bloco(No *bloco, Variavel *listaVar, Funcao *listaFunc) {
    while (bloco != NULL) {
        executar_linha(bloco->tk, listaVar, bloco, listaFunc);
        bloco = bloco->prox;
    }
}

void executar_while(Token *linha, Variavel *listaVar, No *L, Funcao *listaFunc) {
    No *blocoDeExec = NULL, *auxBlocoExec = NULL;
    Token *auxLinha = L->tk;
    Variavel *varsWhile = NULL, *auxVar;
    char comparador[3] = "";
    int qntdTab = 0, cont = 0;

    // Inicializar a vari�vel `varsWhile` corretamente
    varsWhile = NULL; // Inicializa como NULL

    // Para pular o token "while"
    auxLinha = auxLinha->prox;

    // Pegar vari�veis da condi��o e o comparador
    while (auxLinha != NULL && auxLinha->token[0] != ':') {
        auxVar = listaVar;

        if (ehDelimitador(auxLinha->token[0])) {
            // Melhorar a l�gica para operadores de compara��o como ==, !=, >=, <=
            if (auxLinha->prox != NULL && (strcmp(auxLinha->prox->token, "=") == 0)) {
                snprintf(comparador, 3, "%s=", auxLinha->token); // Exemplo: ">=" ou "<="
                auxLinha = auxLinha->prox; // Avan�ar mais um token
            } else {
                snprintf(comparador, 2, "%s", auxLinha->token); // Operadores simples
            }
        } else if (isInteger(auxLinha->token) || isFloat(auxLinha->token)) {
            // Criar uma nova vari�vel se for n�mero e adicionar a `varsWhile`
            Variavel *nova_variavel = (Variavel*)malloc(sizeof(Variavel));
            strcpy(nova_variavel->nome, auxLinha->token);

            if (strchr(auxLinha->token, '.') != NULL) {
                nova_variavel->valor.FLOAT = atof(auxLinha->token);
                nova_variavel->tipoAtual = FLOAT;
            } else {
                nova_variavel->valor.INT = atoi(auxLinha->token);
                nova_variavel->tipoAtual = INT;
            }

            // Inserir nova vari�vel no `varsWhile`
            nova_variavel->prox = NULL;
            if (varsWhile == NULL) {
                varsWhile = nova_variavel;
            } else {
                Variavel *percorrer = varsWhile;
                while (percorrer->prox != NULL) {
                    percorrer = percorrer->prox;
                }
                percorrer->prox = nova_variavel;
            }
        } else {
            // Procurar vari�vel em `listaVar`
            while (auxVar != NULL && strcmp(auxLinha->token, auxVar->nome) != 0) {
                auxVar = auxVar->prox;
            }

            if (auxVar != NULL) {
                // Adicionar vari�vel ao `varsWhile`
                if (varsWhile == NULL) {
                    varsWhile = auxVar;
                } else {
                    Variavel *percorrer = varsWhile;
                    while (percorrer->prox != NULL) {
                        percorrer = percorrer->prox;
                    }
                    percorrer->prox = auxVar;
                }
            } else {
                printf("Vari�vel '%s' n�o encontrada.\n", auxLinha->token);
            }
        }
        auxLinha = auxLinha->prox;
    }

    // Encontrar o bloco de execu��o
    linha = L->prox->tk;
    qntdTab = 0; // Inicializa qntdTab
    while (linha != NULL && (linha->token[0] == '\t' || linha->token[0] == '\0' || linha->token[0] == ' ')) {
        qntdTab++;
        linha = linha->prox;
    }

    auxBlocoExec = L->prox;
    while (L != NULL) {
        cont = 0;
        while (linha != NULL && (linha->token[0] == '\t' || linha->token[0] == '\0' || linha->token[0] == ' ')) {
            cont++;
            linha = linha->prox;
        }

        if (cont == qntdTab) {
            if (blocoDeExec == NULL) {
                blocoDeExec = auxBlocoExec;
            } else {
                auxBlocoExec->prox = L;
                auxBlocoExec = auxBlocoExec->prox;
            }
            L = L->prox;
        } else {
            break; // Parar se a indenta��o mudar
        }
    }

    // Executar o bloco enquanto a condi��o for verdadeira
    while (realizar_comparacao(varsWhile->tipoAtual, varsWhile->valor, varsWhile->prox->tipoAtual, varsWhile->prox->valor, comparador)) {
        executar_bloco(blocoDeExec, varsWhile, listaFunc);
    }

    // Libera��o de mem�ria para vari�veis tempor�rias (evita vazamento de mem�ria)
    Variavel *temp;
    while (varsWhile != NULL) {
        temp = varsWhile;
        varsWhile = varsWhile->prox;
        free(temp);
    }
}
