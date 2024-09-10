#include <string.h>
#define MAX_TOKEN 99

typedef union{
	char STR[MAX_TOKEN];
	int INT;
	float FLOAT;
}Tipos;

typedef enum{
	INT,
	FLOAT,
	STRING
}tipo_atual;

typedef struct Variavel {
    char nome[MAX_TOKEN];			// Nome da variavel
	Tipos valor;					// Valor da variavel
	tipo_atual tipoAtual;			// Tipo atual
    struct Variavel *prox;         // Ponteiro para a proxima variavel na lista
} Variavel;

typedef struct Funcao {
    char nome[MAX_TOKEN];          // Nome da funcao
    Variavel *parametros;          // Lista de parametros da funcao
    struct Funcao *prox;           // Ponteiro para a proxima funcao na lista
    No *starts;
} Funcao;


void processar_tokens(No *lista, Variavel **listaVar, Funcao **listaFunc) {
    No *atual = lista;
    Variavel *parm = NULL, *aux = NULL;
    char nomeFunc[MAX_TOKEN];

    while (atual != NULL) {
        Token *tk = atual->tk;
        while (tk != NULL) {

            // Identificar função
            if (strcmp(tk->token, "def") == 0 && tk->prox != NULL) {
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
                            if (!ehDelimitador(tk->token[0]) && tk->token[0] != '\0') {
                                Variavel *nova_var = (Variavel*) malloc(sizeof(Variavel));
                                strcpy(nova_var->nome, tk->token);
                                nova_var->prox = NULL;

                                if (parm == NULL) { // Primeira variável
                                    parm = nova_var;
                                    aux = parm;  
                                } else { // Próxima variável
                                    aux->prox = nova_var;  
                                }
                                aux = nova_var;  // Atualiza o auxiliar para a última variável inserida
                            }
                            tk = tk->prox;
                        }
                    }

                    if (nomeFunc[0] != '\0') { // Verificação adicional
                        inserir_funcao(listaFunc, nomeFunc, parm, atual);
                    }
                    
                    // Reseta `parm` e `aux` para o próximo processamento de função
                    parm = NULL;
                    aux = NULL;
                }
            }
            // Identificar variável
            else if (tk->prox != NULL) {
				int Found=0;
				if(*listaVar!=NULL){
					Variavel *auxLV = (*listaVar);
					while(auxLV!=NULL && strcmp(auxLV->nome,tk->token)!=0){
						auxLV = auxLV->prox;
					}
					if(auxLV!=NULL){
						Found++;	
					}
				}
				if(!Found){
					Token *prox_tk = tk->prox;
	
	                // Ignorar espaços em branco
	                while (prox_tk != NULL && strcmp(prox_tk->token, " ") == 0) {
	                    prox_tk = prox_tk->prox;
	                }
	
	                if (prox_tk != NULL && strcmp(prox_tk->token, "=") == 0) {
	                    prox_tk = prox_tk->prox;
	                    // Ignorar espaços em branco após "="
	                    while (prox_tk != NULL && strcmp(prox_tk->token, " ") == 0 || prox_tk->token[0] == '\0' || strcmp(prox_tk->token, "=") == 0) {
	                        prox_tk = prox_tk->prox;
	                    }
	
	                    if (prox_tk != NULL) {
	                        inserir_variavel(listaVar, tk->token, prox_tk->token);
	                        tk = prox_tk;  // Avança para o próximo token após o valor
	                    }
	                }	
				}
			}

            tk = tk->prox;
        }
        atual = atual->prox;
    }
}

void inserir_variavel(Variavel **listaVar, char nome[], char valor[]) {
    if(strcmp(nome,"")!=0){
    	float vf;
    	int vi;
    	Variavel *nova_variavel = (Variavel*) malloc(sizeof(Variavel));
    	strcpy(nova_variavel->nome, nome);
    	nova_variavel->prox = NULL;
    	
		if(valor[0]=='\''){
    		strcpy(nova_variavel->valor.STR, valor);
    		nova_variavel->tipoAtual=STRING;
    	}
    	else{
			// ENT EH FLOAT
			if(strchr(valor, '.') != NULL){
				nova_variavel->valor.FLOAT=atof(valor);
				nova_variavel->tipoAtual=FLOAT;
			}
			// ENT EH INT
			else{
				nova_variavel->valor.INT=atoi(valor);
				nova_variavel->tipoAtual=INT;
			}
    	}

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


void inserir_funcao(Funcao **listaFunc, char nome[], Variavel *parametros, No *linhaStart) {
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
    nova_funcao->starts=linhaStart;

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
        printf("- %s = ", atual->nome);
        switch(atual->tipoAtual){
        	case INT: printf("%d INT\n", atual->valor.INT); break;
        	case FLOAT: printf("%f FLOAT\n", atual->valor.FLOAT); break;
        	case STRING: printf("%s STR\n", atual->valor.STR); break;
        }
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



