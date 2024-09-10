#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "variaveis.h"
#include "insercao.h"

Variavel procurar_variavel_print(Variavel *listaVar, char nome_variavel_a_ser_procurada[]) {
	Variavel aux;
	// Já é atribuído o nome de undefined para o caso de não encontrar a variável na lista de variáveis
	strcpy(aux.nome, "undefined");
	// Verifica se a variável a ser procurada está na lista
	while(listaVar != NULL) {
		// Se encontrar a variável, vai copiar para aux
		if(strcmp(nome_variavel_a_ser_procurada, listaVar->nome) == 0) {
			// Copia o nome
			strcpy(aux.nome, listaVar->nome);
			
			// Copia o valor e o tipo
			if(listaVar->tipoAtual == INT) {
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

Variavel encontrar_a_variavel_apos_o_percent(Token *linha, Variavel *listaVar, int posicao) {
	// Vai chegar no segundo "
	char nome_variavel_encontrada[10]; // É o nome da variável que está após o %
	char literal_string[40]; // Essa variável será necessária quando, por exemplo, printf("%s", "7")
	Variavel var; // É a variável que será retornada
	int encontrou_a_variavel = 0;
	while(linha != NULL && strcmp(linha->token, "\"") != 0)
		linha = linha->prox;
	if(linha != NULL) {
		linha = linha->prox; // Pular o "
		
		//Agora precisa-se ultrapassar o % e o ( em print("..." % (...))
		while(linha != NULL && strcmp(linha->token, "(") != 0)
			linha = linha->prox;
		if(linha != NULL)
			linha = linha->prox; // Estamos pulando o (
		
		// Agora precisa andar até o argumento que está em int posição
		int i = 0;
		while(i < posicao) {
			linha = linha->prox;
			if(strcmp(linha->token, ",") != 0 && strcmp(linha->token, "\0") != 0 && strcmp(linha->token, " ") != 0 && strcmp(linha->token, ")")) { 
			// Esse if é necessário para não contabilizar as  vírgulas, \0 , espaços em branco e )
				i++;
			}
			
		}
		if(strcmp(linha->token, "\"") == 0) {
			// Caso seja, por exemplo, printf("%s", "7")
			linha = linha->prox;
			strcpy(literal_string, linha->token);
			linha = linha->prox;
			while(strcmp(linha->token, "\"") != 0) {
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
	//printf("\nNome da variavel encontrada: %s", var.nome);
	return var;
	
	
}



char* gerar_linha_print2(Token *linha, Variavel *listaVar) {
	
	char print_gerado[MAX_LINHA]; // É a string que será retornada
	strcpy(print_gerado, ""); // Retornará uma string vazia caso aux == NULL ou não seja um print
	Token *aux = linha;
	Token *aux2; // É a variável que irá procurar os argumentos após o % em print("..." % (...))
	int contador_posicao = 0; // Irá contar quantos %d %s %f tem, para saber qual deve ser procurado após o %
	
	// Esse primeiro if irá verificar se a variável que guarda os tokens não é NULL e se realmente é uma linha que tem print
	if(aux != NULL && strcmp(aux->token, "print") == 0) {
		// Todos os prints tem essa estrutura print("..." % )
		// Caso entre aqui, a condição de parada será chegar no segundo "
		// Para isso, precisa-se pular o ( e " primeiro, porque senão irá parar nesse primeiro "
		while(strcmp(aux->token, "\"") != 0)
			aux = aux->prox;
		aux = aux->prox; // O token que estava armazenado ao sair do while era o ", então já pulamos um pra frente	
		
		// A condição de parada desse while será o próximo " , pois o que vier depois dele só é necessário
		// para descobrir quais os valores dos %d %s %f caso tenham
		while(strcmp(aux->token, "\"") != 0) {
			
			// Aqui é onde será tratado caso encontre %d %s %f
			if(strcmp(aux->token, "%d") == 0 || strcmp(aux->token, "%s") == 0 || strcmp(aux->token, "%f") == 0 ) {
				aux2 = aux;
				Variavel var_encontrada = encontrar_a_variavel_apos_o_percent(aux, listaVar, contador_posicao);
				char tipo_para_string[10];
				if(strcmp(var_encontrada.nome, "undefined") != 0) {
						if(var_encontrada.tipoAtual == INT) {
						sprintf(tipo_para_string, "%d", var_encontrada.valor.INT);
						strcat(print_gerado, tipo_para_string);
						strcat(print_gerado, " ");
					} else if (var_encontrada.tipoAtual == FLOAT) {
						sprintf(tipo_para_string, "%f", var_encontrada.valor.FLOAT);
					
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
				// Aqui é onde concatena na string a ser gerada caso não seja %d %s e %f
				strcat(print_gerado, aux->token);
				strcat(print_gerado, " ");
			}
			//printf("%s", aux->token);
			aux = aux->prox;
		}

		}

	return print_gerado;
}



void gerar_todas_as_linhas_print(No *lista, Variavel *listaVar) {
	int contador_de_print = 0;
	Token *aux = NULL; 
	char *print_gerado;
	
	while(lista != NULL) {
		//printf("\nEntrou!");
		aux = lista->tk;
		while(aux != NULL) {
			if(strcmp(aux->token, "print") == 0) {
				
				contador_de_print++;
				print_gerado = gerar_linha_print2(aux, listaVar);
				printf("\n%s", print_gerado);
			}
			
			aux = aux->prox;
		}
		lista = lista->prox;
		
	}
	
}

