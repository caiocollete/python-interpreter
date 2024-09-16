#include "listaGen.h"
void executar_while(Token *linha, Variavel *listaVar, No *L, Funcao *listaFunc);

void executar_linha(Token *linha, Variavel *listaVar, No *bloco, Funcao *listaFunc){
 	while(linha!=NULL){
 		if(strcmp(linha->token,"while")==0){
 			executar_while(linha,listaVar,bloco,listaFunc);
 		}
 		if(strcmp(linha->token,"if")==0){
 		}
 		if(strcmp(linha->token,"elif")==0){
 		}
 		if(strcmp(linha->token,"else")==0){
 		}
 		if(strcmp(linha->token,"for")==0){
 		}	
 		else if (!ehDelimitador(linha->token[0])) {
            // Verificar se o token é uma variável
            Variavel *var = procurar_variavel(listaVar, linha->token);
            
            // Se for uma variável existente
            if (strcmp(var->nome, "undefined") != 0) {
                linha = linha->prox; // mover para o próximo token (que pode ser um operador de atribuição)

                // Se for uma atribuição (ex: "=")
                if (linha != NULL && strcmp(linha->token, "=") == 0) {
                    linha = linha->prox; // mover para o próximo token que é o valor a ser atribuído

                    // Agora, é necessário avaliar o valor e atribuir à variável
                    if (linha != NULL) {
                    		// Parse linha to ListaGen
                    	    ListaGen *listaGen = ParseListaEncadeada(linha);
                    	    // RESOLVE A EXPRESSAO DA LISTA GEN
                    	    var->valor.FLOAT = ResolveLista(listaGen);
                    }
                }
            } 
			else {
                // Caso contrário, pode ser um comando ou uma função desconhecida
                printf("Comando ou variável '%s' não reconhecido(a).\n", linha->token);
            }
 		}
 	}
}

// funcao que executa o bloco de codigo dentro do while
void executar_bloco(No *bloco, Variavel *listaVar, Funcao *listaFunc) {
    // Percorre cada linha do bloco e executa a funcao correspondente
    while (bloco != NULL) {
        executar_linha(bloco->tk, listaVar, bloco, listaFunc); // TEMOS QUE FAZER A LINHA AINDA
        bloco = bloco->prox;
    }
}

void executar_while(Token *linha, Variavel *listaVar, No *L, Funcao *listaFunc) {
    No *blocoDeExec,*auxBlocoExec;
	Token *auxLinha;
	Variavel *varsWhile,*auxVar;
	char comparador[3];
	int qntdTab, cont;
	
	auxLinha = L->tk;
	
	if(strcmp(auxLinha->token,"while")!=0){	
		while(strcmp(auxLinha->token,"while")!=0){
			auxLinha=auxLinha->prox;
		}
	}
	
	while(auxLinha->token[0]!=':'){ // pegar variaveis
		auxVar = listaVar;
		
		if(!ehDelimitador(auxLinha->token[0])){
			comparador[0]=auxLinha->token[0];
			if(!ehDelimitador(auxLinha->prox->token[0]) && auxLinha->prox->token[0] != ':'){
				comparador[1]=auxLinha->prox->token[1];
				comparador[2]='\0';
			}	
			else if(!ehDelimitador(auxLinha->prox->prox->token[0]) && auxLinha->prox->prox->token[0] != ':'){
				comparador[1]=auxLinha->prox->prox->token[1];
				comparador[2]='\0';
			}
			else comparador[1]='\0';
		}
		
		
		else if(isInteger(auxLinha->token) || isFloat(auxLinha->token)){
			Variavel *nova_variavel = (Variavel*)malloc(sizeof(Variavel));
			
			strcpy(nova_variavel->nome,auxLinha->token);
			
			if(strchr(auxLinha->token, '.') != NULL){
				nova_variavel->valor.FLOAT=atof(auxLinha->token);
				nova_variavel->tipoAtual=FLOAT;
			}
			else{
				nova_variavel->valor.INT=atoi(auxLinha->token);
				nova_variavel->tipoAtual=INT;
			}
			
			if(strcmp(auxLinha->token,listaVar->nome)==0){
				
		    	if (varsWhile == NULL) {
		        	varsWhile = nova_variavel;
		    	} 
				else{
			        Variavel *percorrer = varsWhile;
			        while (percorrer->prox != NULL) {
			            percorrer = percorrer->prox;
			        }
		        	percorrer->prox = nova_variavel;
	    		}
				
			}
			
		}
		else{
			while(strcmp(auxLinha->token,auxVar->nome)!=0 && auxVar!=NULL){
				auxVar=auxVar->prox;
			}
			
			
			
			if(strcmp(auxLinha->token,auxVar->nome)==0){
				
		    	if (varsWhile == NULL) {
		        	varsWhile = auxVar;
		    	} 
				else{
			        Variavel *percorrer = varsWhile;
			        while (percorrer->prox != NULL) {
			            percorrer = percorrer->prox;
			        }
		        	percorrer->prox = listaVar;
	    		}
				
			}
			else printf("variavel nao encontrada");
		}
			auxLinha=auxLinha->prox;
	}
	
	
	linha=L->prox->tk; // para a linha do bloco
	
	while(linha->token[0]=='\t' || linha->token[0]=='\0' || linha->token[0]==' '){
		qntdTab++;
		linha=linha->prox;
	}
	blocoDeExec=auxBlocoExec;
	auxBlocoExec=L->prox;
	
	
	//contar quantas linhas faz parte do bloco e adc no bloco de Execucao
	int continuar=1;
	while(L!=NULL && continuar==1){
		cont=0;
		while(linha->token[0]=='\t' || linha->token[0]=='\0' || linha->token[0]==' '){
			qntdTab++;
			linha=linha->prox;
		}	
		if(cont==qntdTab){
			auxBlocoExec->prox=L;
			auxBlocoExec=auxBlocoExec->prox;
			L=L->prox;
		}
		else continuar=0;
	}
    
    
    
    while (realizar_comparacao(varsWhile->tipoAtual, varsWhile->valor, varsWhile->prox->tipoAtual, varsWhile->prox->valor, comparador)) {
        // Enquanto a condicao for verdadeira, executamos o bloco de codigo
        executar_bloco(blocoDeExec, varsWhile, listaFunc);
    }
}

