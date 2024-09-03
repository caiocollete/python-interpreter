#include <string.h>
#define MAX_TOKEN 99

typedef struct Variavel {
    char nome[MAX_TOKEN];			// Nome da vari�vel
	int valor;         				// Valor da vari�vel
    struct Variavel *prox;         // Ponteiro para a pr�xima vari�vel na lista
} Variavel;

typedef struct Funcao {
    char nome[MAX_TOKEN];          // Nome da fun��o
    Variavel *parametros;          // Lista de par�metros da fun��o
    struct Funcao *prox;           // Ponteiro para a pr�xima fun��o na lista
} Funcao;


void processar_tokens(No *lista, Variavel **listaVar, Funcao **listaFunc) {
    No *atual = lista;
    Variavel *parm = NULL, *aux = NULL;
    Token *auxTk;
    char nomeFunc[MAX_TOKEN];

    while (atual != NULL) {
        Token *tk = atual->tk;
        while (tk != NULL) {
            // Identificar fun��o
            if (strcmp(tk->token, "def") == 0 && tk->prox != NULL) {
                // Avan�a para o nome da fun��o
                tk = tk->prox;
                
                // Ignora espa�os em branco
                while (tk != NULL && strcmp(tk->token, " ") == 0) {
                    tk = tk->prox;
                }

                if (tk != NULL) {
                    strcpy(nomeFunc, tk->token); // Armazena o nome da fun��o

                    // Verifica se a fun��o possui par�metros
                    if (tk->prox != NULL && strcmp(tk->prox->token, "(") == 0) {
                        tk = tk->prox->prox; // Avan�a para o primeiro poss�vel par�metro

                        while (tk != NULL && strcmp(tk->token, ")") != 0) {
                            // Ignora espa�os em branco e v�rgulas
                            if (!ehDelimitador(tk->token[0]) && tk->token[0]!='\0') {
                                Variavel *nova_var = (Variavel*) malloc(sizeof(Variavel));
                                strcpy(nova_var->nome, tk->token);
                                nova_var->prox = NULL;

                                if (parm == NULL) { // Primeira vari�vel
                                    parm = nova_var;
									aux = parm;  
                                } 
								else { // Pr�xima vari�vel
                                    aux->prox = nova_var;  
                                }
                                aux = nova_var;  // Atualiza o auxiliar para a �ltima vari�vel inserida
                            }
                            tk = tk->prox;
                        }
                    }

                    if (nomeFunc[0] != '\0') { // Verifica��o adicional
                        inserir_funcao(listaFunc, nomeFunc, parm);
                    }
                    
        	        // Reseta `parm` e `aux` para o pr�ximo processamento de fun��o
                    parm = NULL;
                    aux = NULL;
                }
            }
            // Identificar vari�vel
             else if (tk->prox != NULL) {
                Token *prox_tk = tk->prox;
                while (prox_tk != NULL && strcmp(prox_tk->token, " ") == 0) {
                    prox_tk = prox_tk->prox;
                }
                if (prox_tk != NULL && strcmp(prox_tk->token, "=") == 0) {
                    inserir_variavel(listaVar, tk->token);
                }
            }
            tk = tk->prox;
        }
        atual = atual->prox;
    }
}



void inserir_variavel(Variavel **listaVar, char nome[]) {
    
    if(strcmp(nome,"")!=0){
    	Variavel *nova_variavel = (Variavel*) malloc(sizeof(Variavel));
    	strcpy(nova_variavel->nome, nome);
    	nova_variavel->prox = NULL;

    	if (*listaVar == NULL) {
        	*listaVar = nova_variavel;
    	} 
		else{
	        Variavel *percorrer = *listaVar;
	        while (percorrer->prox != NULL) {
	            percorrer = percorrer->prox;
        	}
        	percorrer->prox = nova_variavel;
    	}	
    }
}


void inserir_funcao(Funcao **listaFunc, char nome[], Variavel *parametros) {
    Funcao *nova_funcao = (Funcao*) malloc(sizeof(Funcao));
    if (nova_funcao == NULL) {
        // Tratamento de erro de aloca��o de mem�ria
        fprintf(stderr, "Erro de aloca��o de mem�ria para nova fun��o.\n");
        exit(1);
    }
    strncpy(nova_funcao->nome, nome, MAX_TOKEN - 1);
    nova_funcao->nome[MAX_TOKEN - 1] = '\0';  // Garantir termina��o da string
    nova_funcao->parametros = parametros;
    nova_funcao->prox = NULL;

    if (*listaFunc == NULL) {
        *listaFunc = nova_funcao;
    } else {
        Funcao *percorrer = *listaFunc;
        while (percorrer->prox != NULL) {
            percorrer = percorrer->prox;
        }
        percorrer->prox = nova_funcao;
    }
}

void exibir_variaveis(Variavel *listaVar) {
    if (listaVar == NULL) {
        printf("Nenhuma vari�vel identificada.\n");
        return;
    }

    Variavel *atual = listaVar;
    printf("Variaveis identificadas:\n");
    while (atual != NULL){
        printf("- %s\n", atual->nome);
        atual = atual->prox;
    }
}

void exibir_funcoes(Funcao *listaFunc) {
    if (listaFunc == NULL) {
        printf("Nenhuma funcao identificada.\n");
        return;
    }

    Funcao *atual = listaFunc;
    printf("Funcoes identificadas:\n");
    while (atual != NULL) {
        printf("- %s(", atual->nome);

        // Exibir par�metros da fun��o
        Variavel *parametro = atual->parametros;
        while (parametro != NULL) {
            printf("%s", parametro->nome);
            if (parametro->prox != NULL) {
                printf(", ");
            }
            parametro = parametro->prox;
        }
        printf(")\n");

        atual = atual->prox;
    }
}



