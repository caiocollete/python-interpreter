#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "insercao.h"
#include "variaveis.h"

int main() {
	No *lista;
	Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
	
	inicializar(&lista);
	inserir_a_partir_de_arquivo(lista, "ex.py");
	
	//Processar a lista para identificar variaveis e funcoes
	processar_tokens(lista, &listaVar, &listaFunc);
    
	//exibir_lista(lista);
	exibir_variaveis(listaVar);
    //exibir_funcoes(listaFunc);
}
