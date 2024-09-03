#include <string.h>
#define MAX_TOKEN 99

typedef struct Variavel {
    char nome[MAX_TOKEN];			// Nome da variï¿½vel
	int valor;         				// Valor da variï¿½vel
    struct Variavel *prox;         // Ponteiro para a prï¿½xima variï¿½vel na lista
} Variavel;

typedef struct Funcao {
    char nome[MAX_TOKEN];          // Nome da funï¿½ï¿½o
    Variavel *parametros;          // Lista de parï¿½metros da funï¿½ï¿½o
    struct Funcao *prox;           // Ponteiro para a prï¿½xima funï¿½ï¿½o na lista
} Funcao;


void processar_tokens(No *lista, Variavel **listaVar, Funcao **listaFunc) {
    No *atual = lista;
    Variavel *parm = NULL, *aux = NULL;
    Token *auxTk;
    char nomeFunc[MAX_TOKEN];

    while (atual != NULL) {
        Token *tk = atual->tk;
        while (tk != NULL) {
            // Identificar função
            if (strcmp(tk->token, "def") == 0 && tk->prox != NULL) {
                // Avança para o nome da função
                tk = tk->prox;
                
                // Ignora espaços em branco
                while (tk != NULL && strcmp(tk->token, " ") == 0) {
                    tk = tk->prox;
                }

                if (tk != NULL) {
                    strcpy(nomeFunc, tk->token); // Armazena o nome da função

                    // Verifica se a função possui parâmetros
                    if (tk->prox != NULL && strcmp(tk->prox->token, "(") == 0) {
                        tk = tk->prox->prox; // Avança para o primeiro possível parâmetro

                        while (tk != NULL && strcmp(tk->token, ")") != 0) {
                            // Ignora espaços em branco e vírgulas
                            if (!ehDelimitador(tk->token[0]) && tk->token[0]!='\0') {
                                Variavel *nova_var = (Variavel*) malloc(sizeof(Variavel));
                                strcpy(nova_var->nome, tk->token);
                                nova_var->prox = NULL;

                                if (parm == NULL) { // Primeira variável
                                    parm = nova_var;
									aux = parm;  
                                } 
								else { // Próxima variável
                                    aux->prox = nova_var;  
                                }
                                aux = nova_var;  // Atualiza o auxiliar para a última variável inserida
                            }
                            tk = tk->prox;
                        }
                    }

                    if (nomeFunc[0] != '\0') { // Verificação adicional
                        inserir_funcao(listaFunc, nomeFunc, parm);
                    }
                    
        	        // Reseta `parm` e `aux` para o próximo processamento de função
                    parm = NULL;
                    aux = NULL;
                }
            }
            // Identificar variável
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
        // Tratamento de erro de alocação de memória
        fprintf(stderr, "Erro de alocação de memória para nova função.\n");
        exit(1);
    }
    strncpy(nova_funcao->nome, nome, MAX_TOKEN - 1);
    nova_funcao->nome[MAX_TOKEN - 1] = '\0';  // Garantir terminação da string
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
        printf("Nenhuma variável identificada.\n");
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

        // Exibir parâmetros da função
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



