#define MAX_LINHA 100
#define MAX_TOKEN 99

#define ESPACO ' '
#define PARENTESEAB '('
#define PARENTESEFECHA ')'
#define VIRGULA ','
#define CHAVEAB '{'
#define CHAVEFECHA '}'
#define COLCHETEAB '['
#define COLCHETEFECHA ']'
#define DIVISOR '/'
#define ASPAS '"'
#define ASPASSIMPLES '\''
#define ASTERISCO '*'
#define SOMA '+'
#define SUBTRACAO '-'
#define IGUAL '='
#define NULO '\0'

typedef struct Token{
	char token[MAX_TOKEN];
	struct Token *prox;
} Token;

typedef struct No{
	struct No *ant, *prox;
	Token *tk;
} No;

char ehDelimitador(char c) {
    return c == PARENTESEAB || c == PARENTESEFECHA ||
           c == VIRGULA || c == CHAVEAB || c == CHAVEFECHA ||
           c == COLCHETEAB || c == COLCHETEFECHA ||
           c == DIVISOR || c == ASTERISCO ||
           c == SOMA || c == SUBTRACAO || c == IGUAL;
}

int ehEspaco(char c){
	return c == ESPACO;
}

void inicializar_com_null(No **lista) {
	*lista = NULL;	
}

void inicializar(No **lista) {
	*lista = (No*) malloc(sizeof(No));
	(*lista)->ant = (*lista)->prox =NULL;
	(*lista)->tk = NULL;		
}

void inserir_a_partir_de_arquivo(No *lista, char nome_arquivo[]) {
	FILE *fp;
	char str[MAX_LINHA];
	fp = fopen(nome_arquivo, "r");
	if(fp != NULL) {
		
		while(fgets(str, MAX_LINHA, fp) != NULL) {
			inserir_no_lista_vertical(str, lista);
		}
	}
}

void separar_string(char string_original[], char nova_string[], char token_retornado[], char delimitador[]) {
	int cont = 0;
	// Gera o token
	while(cont < MAX_TOKEN - 1 && string_original[cont] != '\0' &&  !ehDelimitador(string_original[cont]) && string_original[cont] != ' ') {
				if(string_original[cont]=='\''){
					token_retornado[cont] = string_original[cont];
					cont++;
					while(string_original[cont]!='\''){
						token_retornado[cont] = string_original[cont];
						cont++;
					}
					token_retornado[cont] = string_original[cont];
					cont++;
				}
				else{
					token_retornado[cont] = string_original[cont];
					cont++;
				}
	}
	token_retornado[cont] = '\0';
	
	// Gera a string resultante (nova_string = string original - token_retornado)
	// Se a string_original parou com um espaço, esse espaço é eliminado na nova_string
	int i = 0;
	if(string_original[cont] == ' ')
		cont++;
	if(ehDelimitador(string_original[cont])){
		delimitador[0]=string_original[cont];
		delimitador[1]='\0';
		cont++;
	}
	else delimitador[0]='\0';
		
	while(string_original[cont] != '\0') {
		nova_string[i] = string_original[cont];
		i++;
		cont++;
	}
	nova_string[i] = '\0';
	
}


void inserir_no_lista_vertical(char linha[], No *lista) {
	
	No *nova_caixa = (No*) malloc(sizeof(No));
	nova_caixa->ant = nova_caixa->prox = nova_caixa->tk = NULL;
	
	char token_retornado[MAX_TOKEN], delimitador[2];
	char nova_string[100];
	
	int cont;
	
	if(lista->tk != NULL) {
		// Se entrar aqui, significa que a lista vertical não está vazia
		while(lista->prox != NULL) 
			lista = lista->prox;	
		lista->prox = nova_caixa;
		nova_caixa->ant = lista;
		lista = nova_caixa;
		
		
	} 
	else {
		lista->tk = NULL;
	}
	
	while(linha[0] != '\0'){
			separar_string(linha, nova_string, token_retornado, delimitador);
			linha = nova_string;
		
			inserir_token_lista_horizontal(&(lista->tk), token_retornado);
			if(delimitador[0]!='\0' && linha[0]!='\0'){
				inserir_token_lista_horizontal(&(lista->tk), delimitador);
			}
	}
}

void inserir_token_lista_horizontal(Token **token, char token_a_inserir[]) {
	Token *aux = (Token*) malloc(sizeof(Token));
	Token *percorrer = *token;
	strcpy(aux->token, token_a_inserir);
	aux->prox = NULL;
	if(*token == NULL) {
		*token = aux;
	} else {
		while(percorrer->prox != NULL) {
			percorrer = percorrer->prox;
		}
		percorrer->prox = aux;
		
	}
}


void exibir_lista(No *lista) {
	Token *aux;
	if(lista->tk != NULL) {
		while(lista != NULL) {
			aux = lista->tk;
			while(aux != NULL) {
				printf("-> %s ", aux->token);
				aux = aux->prox;
			}
			printf("\n");
			lista = lista->prox;
		}
	}
	
}

