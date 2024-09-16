#include <math.h>

// Estruturas da lista generalizada
struct reg_lista {
    struct listagen *head;
    struct listagen *tail;
};

union info_lista {
    float valor;
    struct reg_lista lista;
    char operador;
};

struct listagen {
    char terminal; // 1 se é um valor, 0 se é uma lista
    union info_lista no;
};
typedef struct listagen ListaGen;

// Função para criar um nó terminal (valor numérico)
ListaGen *CriaValor(float valor) {
    ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
    L->terminal = 1;
    L->no.valor = valor;
    return L;
}

// Função para criar um nó terminal (operador ou função)
ListaGen *CriaOperador(char operador) {
    ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
    L->terminal = 1;
    L->no.operador = operador;
    return L;
}

// Função para verificar se a lista é nula
char Nula(ListaGen *L) {
    return L == NULL;
}

// Verifica se o nó é um átomo (valor ou operador)
char Atomo(ListaGen *L) {
    return !Nula(L) && L->terminal;
}

// Função para criar uma sublista (não terminal)
ListaGen *Cons(ListaGen *H, ListaGen *T) {
    ListaGen *L = (ListaGen*)malloc(sizeof(ListaGen));
    L->terminal = 0;
    L->no.lista.head = H;
    L->no.lista.tail = T;
    return L;
}

// Função para realizar uma operação básica (+, -, *, /, ^)
float ExecutaOperacao(char operador, float op1, float op2) {
    switch (operador) {
        case '+': return op1 + op2;
        case '-': return op1 - op2;
        case '*': return op1 * op2;
        case '/': return op1 / op2;
        case '^': return pow(op1, op2);
        default: return 0;
    }
}

// Função para avaliar funções como sin, cos, sqrt
float ExecutaFuncao(char *funcao, float valor) {
    if (strcmp(funcao, "sin") == 0) return sin(valor);
    if (strcmp(funcao, "cos") == 0) return cos(valor);
    if (strcmp(funcao, "sqrt") == 0) return sqrt(valor);
    return 0;
}

// Função para pular tokens vazios
Token* ProximoTokenValido(Token* token) {
    while (token && token->token[0] == '\0') {
        token = token->prox;
    }
    return token;
}

// Parsing básico: Identifica o próximo token da lista encadeada e cria a lista generalizada
ListaGen* ParseListaEncadeada(Token* token, Variavel *listaVar) {
    ListaGen *L = NULL;
    token = ProximoTokenValido(token);

    while (token != NULL) {
        char* t = token->token;

        // Se for numero, cria um atomo numerico
        if (isdigit(t[0]) || t[0] == '.') {
            float valor = atof(t);
            token = token->prox;
            return CriaValor(valor);
        }
        else{
        	Variavel *var = procurar_variavel(listaVar, token->token);
        	token=token->prox;
			if(var!=NULL){
				if(var->tipoAtual==INT)
        			return CriaValor(var->valor.INT);
        		if(var->tipoAtual==FLOAT)
        			return CriaValor(var->valor.FLOAT);
        	}
        }

        // Se for operador, cria um átomo de operador
        if (strchr("+-*/^", t[0])) {
            char operador = t[0];
            token = token->prox;
            return CriaOperador(operador);
        }

        // Se for função como sin, cos, sqrt
        if (isalpha(t[0])) {
            char funcao[MAX_TOKEN];
            strcpy(funcao, t);
            token = token->prox; // Avança para o próximo token

            // Espera-se um parêntese depois da função
            if (strcmp(token->token, "(") == 0) {
                token = token->prox; // Avança após o '('
                ListaGen *subexpressao = ParseListaEncadeada(token,listaVar);
                token = token->prox; // Consome o ')'
                return CriaValor(ExecutaFuncao(funcao, subexpressao->no.valor));
            }
        }

        // Se for parêntese abrindo, cria sublista
        if (strcmp(t, "(") == 0) {
            token = token->prox;
            ListaGen *subexpressao = ParseListaEncadeada(token, listaVar);
            token = token->prox; // Consome o ')'
            return subexpressao;
        }

        // Avança para o próximo token se não encontrar algo válido
        token = token->prox;
    }
    return L;
}

// Função recursiva para resolver a lista generalizada
float ResolveLista(ListaGen *L) {
    if (Atomo(L)) {
        return L->no.valor;
    }

    ListaGen *operando1 = L->no.lista.head;
    ListaGen *operador = operando1->no.lista.tail;
    ListaGen *operando2 = operador->no.lista.tail;

    if (Atomo(operador)) {
        return ExecutaOperacao(operador->no.operador, ResolveLista(operando1), ResolveLista(operando2));
    }

    return 0; // Para outras operações ou caso erro
}

// Função para imprimir a lista de tokens (para debug)
void ImprimeTokens(Token* lista) {
    while (lista != NULL) {
        printf("Token: %s\n", lista->token);
        lista = lista->prox;
    }
}

void DestruirLG(ListaGen **L){
    if(*L != NULL) {
        if(Atomo(*L)) free(*L);
        else{
            DestruirLG(&(*L)->no.lista.head);
            DestruirLG(&(*L)->no.lista.tail);
            free(*L);
            *L = NULL;
        }
    }
}
