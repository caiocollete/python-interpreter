void inserir_no_lista_vertical(char linha[], No *lista);
void inserir_token_lista_horizontal(Token **token, char token_a_inserir[]);
void inserir_a_partir_de_arquivo(No *lista, char nome_arquivo[]);

void inserir_no_lista_vertical(char linha[], No *lista) {
	
	No *nova_caixa = (No*) malloc(sizeof(No));
	nova_caixa->ant = nova_caixa->prox = nova_caixa->tk = NULL;
	
	char token_retornado[MAX_TOKEN],token_delimitador[2];
	char nova_string[MAX_TOKEN];
	
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
		//lista vazia
		lista->tk = NULL;
	}
	while(linha[0] != '\0') {
			separar_string(linha, nova_string, token_retornado, token_delimitador);
			linha = nova_string;
		
			inserir_token_lista_horizontal(&(lista->tk), token_retornado);
			if(token_delimitador[0]!=''){
				inserir_token_lista_horizontal(&(lista->tk), token_delimitador);				
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
