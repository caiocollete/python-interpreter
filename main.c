#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "insercao.h"
#include "variaveis.h"
#include "executa.h"

char menu() {
	char esc;
	
	system("cls");
	
	printf("\n==== MENU ====\n");
	printf("[A] - Executar Codigo\n");
	printf("[B] - Exibir RAM\n");
	printf("[C] - Exibe Lista\n\n");
	
	
	printf("-> "); scanf("%c",&esc);
	
	system("cls");
	return tolower(esc);
}

int main() {
	No *lista;
	Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
    char esc;
	
	inicializar(&lista);
	inserir_a_partir_de_arquivo(lista, "ex.py");
	//Processar a lista para identificar variaveis e funcoes
	processar_tokens(lista, &listaVar, &listaFunc);
	
	do{
		switch(menu()){
			case 'b': 	exibir_variaveis(listaVar);
						printf("\n");
						exibir_funcoes(listaFunc);
						break;
			case 'a': executaCodigo(lista,listaVar,listaFunc); break;
			case 'c': exibir_lista(lista);
		}
		scanf("%c",&esc);
		esc=tolower(esc);
	}while(esc!='s');
    
	//exibir_lista(lista);
	//exibir_variaveis(listaVar);
    //exibir_funcoes(listaFunc);
}
