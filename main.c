#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "func/structs.h"
#include "func/delimit.h"
#include "func/init.h"
#include "func/inserts.h"

void separar_string(char string_original[], char nova_string[], char token_retornado[], char tok_delimitador[]) {
	int cont = 0;
	// Gera o token
	while(cont < MAX_TOKEN - 1 && string_original[cont] != '\0' &&  !ehDelimitador(string_original[cont])) {

		token_retornado[cont] = string_original[cont];
		cont++;
	}
	token_retornado[cont] = '\0';
	

	// Gera a string resultante (nova_string = string original - token_retornado)
	// Se a string_original parou com um espaço, esse espaço é eliminado na nova_string
	int i = 0;
	if(ehDelimitador(string_original[cont])){
		if(!ehEspaco(string_orginal[cont])){
			tok_delimitador[0]= string_original[cont];
			tok_delimitador[1]='\0';
			cont++;
		}
		else tok_delimitador=NULL;
	}
	while(string_original[cont] != '\0') {
		nova_string[i] = string_original[cont];
		i++;
		cont++;
	}
	nova_string[i] = '\0';

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

int main() {
	No *lista;
	
	inicializar(&lista);
	inserir_a_partir_de_arquivo(lista, "ex.py");
	exibir_lista(lista);

}



