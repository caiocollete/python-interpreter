#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio2.h>

#include "insercao.h"
#include "variaveis.h"
#include "executa.h"
//#include "print.h"

void gerar_todas_as_linhas_print(No *lista, Variavel *listaVar);
char* gerar_linha_print2(Token *linha, Variavel *listaVar);
Variavel encontrar_a_variavel_apos_o_percent(Token *linha, Variavel *listaVar, int posicao);
Variavel procurar_variavel_print(Variavel *listaVar, char nome_variavel_a_ser_procurada[]);


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

int isInteger(const char *str) {
    char *endptr;
    strtol(str, &endptr, 10);
    return *endptr == '\0';
}

int isFloat(const char *str) {
    char *endptr;
    strtod(str, &endptr);
    return *endptr == '\0';
}


char realizar_comparacao(tipo_atual tipo_operando1, Tipos valor_operando1, tipo_atual tipo_operando2, Tipos valor_operando2, char comparador[]) {
    char comparador_igualdade[3],   // ==
    comparador_diferenca[3],    // !=
    comparador_maior[2],        // >
    comparador_menor[2],        // <
    comparador_maior_igual[3],  // >=
    comparador_menor_igual[3];  // <=

    strcpy(comparador_igualdade, "==");
    strcpy(comparador_diferenca, "!=");
    strcpy(comparador_maior, ">");
    strcpy(comparador_menor, "<");
    strcpy(comparador_maior_igual, ">=");
    strcpy(comparador_menor_igual, "<=");

    // Faz as comparações caso ambos sejam inteiros
    if (tipo_operando1 == INT && tipo_operando2 == INT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.INT == valor_operando2.INT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.INT != valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.INT > valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.INT < valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.INT >= valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.INT <= valor_operando2.INT);

    } else if (tipo_operando1 == FLOAT && tipo_operando2 == FLOAT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.FLOAT == valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.FLOAT != valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.FLOAT > valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.FLOAT < valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.FLOAT >= valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.FLOAT <= valor_operando2.FLOAT);
    } else if (tipo_operando1 == INT && tipo_operando2 == FLOAT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.INT == valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.INT != valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.INT > valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.INT < valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.INT >= valor_operando2.FLOAT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.INT <= valor_operando2.FLOAT);
    } else if (tipo_operando1 == FLOAT && tipo_operando2 == INT) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return (valor_operando1.FLOAT == valor_operando2.INT);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return (valor_operando1.FLOAT != valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (valor_operando1.FLOAT > valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (valor_operando1.FLOAT < valor_operando2.INT);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (valor_operando1.FLOAT >= valor_operando2.INT);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (valor_operando1.FLOAT <= valor_operando2.INT);
    } else if (tipo_operando1 == STRING && tipo_operando2 == STRING) {
        if (strcmp(comparador, comparador_igualdade) == 0)
            return !strcmp(valor_operando1.STR, valor_operando2.STR);
        else if (strcmp(comparador, comparador_diferenca) == 0)
            return !strcmp(valor_operando1.STR, valor_operando2.STR);
        else if (strcmp(comparador, comparador_maior) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) > 0);
        else if (strcmp(comparador, comparador_menor) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) < 0);
        else if (strcmp(comparador, comparador_maior_igual) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) >= 0);
        else if (strcmp(comparador, comparador_menor_igual) == 0)
            return (strcmp(valor_operando1.STR, valor_operando2.STR) <= 0);
    }

    return -1;  // Esse valor é retornado caso não seja possível concluir se uma expressão é verdadeira ou falsa
}



char retornar_valor_de_apenas_uma_comparacao(Token *linha, Variavel *listaVar) {
	// Essa função é responsável por separar os operadores de uma comparação, por exemplo: 1 == 4
	// Ela irá separar os tokens [1, ==, 4] e, após separá-los, irá jogá-los em uma função que retorna o valor dessa comparação
	// E aí a função atual irá retornar o valor

	Tipos valor_do_operando1;
	Tipos valor_do_operando2;
	tipo_atual tipo_do_operando1;
	tipo_atual tipo_do_operando2;
	char comparador_utilizado[3];
	strcpy(comparador_utilizado, "");

	
	// Verifica se a linha não é NULL e se realmente é um if ou elif
	//printf("\neh um if");
	if(linha != NULL && (strcmp(linha->token, "if") == 0 || strcmp(linha->token, "elif") == 0)) {
		
		linha = linha->prox;
		
		// Vai chegar no primeiro token da comparação
		// Esse while é necessário caso haja '\0' ou espaços em branco na linha
		while(linha != NULL && strcmp(linha->token, " ") == 0 || strcmp(linha->token, "\0") == 0) {
			linha = linha->prox;
			
		}
		
		// Esse bloco de código é responsável por pegar o primeiro operando (o operando que está à esquerda do operador, ex:
		// 1 == 2     Esse bloco de código irá pegar o 1
		// Aqui irá pegar o primeiro operando
		// Verifica se é uma string
		if(strcmp(linha->token, "\"") == 0) {
			// Se for uma string, irá armazená-la
		
			tipo_do_operando1 = STRING;
			char string_a_armazenar[MAX_TOKEN];
			strcpy(string_a_armazenar, "");
			linha = linha->prox;
			while(strcmp(linha->token, "\"") != 0) {
				strcat(string_a_armazenar, linha->token);
				strcat(string_a_armazenar, " ");
				linha = linha->prox;
			}
			strcpy(valor_do_operando1.STR, string_a_armazenar);
			printf("Operando1 = %s", valor_do_operando1.STR);
			linha = linha->prox;
		} else {
			// Se não for string, só pode ser um número ou uma variável
			// Se for para o else, então é float, porque não é inteiro e nem string
			if(isInteger(linha->token)) {
				tipo_do_operando1 = INT;
				valor_do_operando1.INT = atoi(linha->token);
				printf("\nOperando1 = %d", valor_do_operando1.INT);
			} else if(isFloat(linha->token)){
				tipo_do_operando1 = FLOAT;
				valor_do_operando1.FLOAT = atof(linha->token);
				printf("\nOperando1 = %f", valor_do_operando1.FLOAT);
			} else {
				Variavel var = procurar_variavel_print(listaVar, linha->token);
				tipo_do_operando1 = var.tipoAtual;
				if(tipo_do_operando1 == FLOAT) {
					valor_do_operando1.FLOAT = var.valor.FLOAT;
				} else if (tipo_do_operando1 == INT) {
					valor_do_operando1.INT = var.valor.INT;
				} else {
					strcpy(valor_do_operando1.STR, var.valor.STR);
				}
				printf("\nOperando1 = %d", valor_do_operando1);
			}
		}
		
		linha = linha->prox;
		// Esse bloco de código irá pegar o operador, em 1 == 2 vai pegar ==
		// Esse while é necessário caso haja '\0' ou espaços em branco na linha após o primeiro operando
		while(linha != NULL && strcmp(linha->token, " ") == 0 || strcmp(linha->token, "\0") == 0) {
			linha = linha->prox;
			
		}
		
		// Esse bloco de código irá armazenar o comparador
		// Como nossa lista está armazenado '\0' e espaços em branco, é necessário verificar um por um senão o código não roda
		// O token == está sendo armazenado como = -> -> = , então ele será verificado primeiro
		//Se entrar no if, significa que é o == e será preciso armazená-lo
		if(strcmp(linha->token, "=") == 0) {
			strcat(comparador_utilizado, linha->token);
			while(linha != NULL && strcmp(linha->token, "=") != 0) {
				linha = linha->prox;
			}
			strcat(comparador_utilizado, linha->token);
			linha = linha->prox;
			linha = linha->prox;
		} else if ((strcmp(linha->token, ">") == 0 || strcmp(linha->token, "<") == 0) && (strcmp(linha->prox->token, "=") != 0)) {
			// O comparador < e > não tem '\0' nem espaço em branco após
			strcat(comparador_utilizado, linha->token);
		} else {
			// Os comparadores restantes agem da mesma forma, tem '\0' ou espaço em branco após eles
			while(linha != NULL && strcmp(linha->token, " ") != 0 && strcmp(linha->token, "\0") != 0) {
			strcat(comparador_utilizado, linha->token);
			linha = linha->prox;
			}
		}

		printf("\nComparador %s", comparador_utilizado);
		
		// Esse próximo bloco de código é responsável por pegar o segundo operando
		linha = linha->prox;
		// Esse while é necessário caso haja '\0' ou espaços em branco na linha após o primeiro operando
		while(linha != NULL && (strcmp(linha->token, " ") == 0 || strcmp(linha->token, "\0") == 0)) {
			linha = linha->prox;
			
		}
		
		if(strcmp(linha->token, "\"") == 0) {
			
			// Se for uma string, irá armazená-la
		
			tipo_do_operando2 = STRING;
			char string_a_armazenar[MAX_TOKEN];
			strcpy(string_a_armazenar, "");
			linha = linha->prox;
			while(strcmp(linha->token, "\"") != 0) {
				strcat(string_a_armazenar, linha->token);
				strcat(string_a_armazenar, " ");
				linha = linha->prox;
			}
			strcpy(valor_do_operando2.STR, string_a_armazenar);
			printf("\nOperando2 = %s", valor_do_operando2.STR);
		} else {
			// Se não for string, só pode ser um número e aí é necessário descobrir se é inteiro ou float
			// Se for para o else, então é float, porque não é inteiro e nem string
			if(isInteger(linha->token)) {
				tipo_do_operando2 = INT;
				valor_do_operando2.INT = atoi(linha->token);
				printf("\nOperando2 = %d", valor_do_operando2.INT);
			} else if(isFloat(linha->token)) {
				tipo_do_operando2 = FLOAT;
				valor_do_operando2.FLOAT = atof(linha->token);
				printf("\nOperando2 = %f", valor_do_operando2.FLOAT);
			} else {
				Variavel var = procurar_variavel_print(listaVar, linha->token);
				tipo_do_operando2 = var.tipoAtual;
				if(tipo_do_operando2 == FLOAT) {
					valor_do_operando2.FLOAT = var.valor.FLOAT;
				} else if (tipo_do_operando2 == INT) {
					valor_do_operando2.INT = var.valor.INT;
				} else {
					strcpy(valor_do_operando2.STR, var.valor.STR);
				}
				printf("\nOperando2 = %d", valor_do_operando2);
			}
		}
		
		
		printf("\n Resultado da operacao: %d", realizar_comparacao(tipo_do_operando1, valor_do_operando1, tipo_do_operando2, valor_do_operando2, comparador_utilizado));
		
		
		
		
		
	}
	
	return realizar_comparacao(tipo_do_operando1, valor_do_operando1, tipo_do_operando2, valor_do_operando2, comparador_utilizado);
}







int main() {
	No *lista;
	Variavel *listaVar = NULL;
    Funcao *listaFunc = NULL;
    char esc;
	
	inicializar(&lista);
	inserir_a_partir_de_arquivo(lista, "teste.py");
	//Processar a lista para identificar variaveis e funcoes
	processar_tokens(lista, &listaVar, &listaFunc);
	
	/*do{
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
		getch();
		
	}while(esc!='s');*/
	
	// TESTE
	
	exibir_lista(lista);
	gerar_todas_as_linhas_print(lista, listaVar);

	while(lista != NULL) {
		printf("\n%c", retornar_valor_de_apenas_uma_comparacao(lista->tk, listaVar));
		lista = lista->prox;
	}
	
	

    
	//exibir_lista(lista);
	//exibir_variaveis(listaVar);
    //exibir_funcoes(listaFunc);
}



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
		if(aux != NULL && strcmp(aux->token, "print") == 0) {
				contador_de_print++;
				print_gerado = gerar_linha_print2(aux, listaVar);
				printf("\n%s", print_gerado);
		}
		
		lista = lista->prox;
	
	}
	
	
}

