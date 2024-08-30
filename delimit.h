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

// Função para verificar se um caractere é um delimitador
int ehDelimitador(char c) {
    return c == ESPACO || c == PARENTESEAB || c == PARENTESEFECHA ||
           c == VIRGULA || c == CHAVEAB || c == CHAVEFECHA ||
           c == COLCHETEAB || c == COLCHETEFECHA ||
           c == DIVISOR || c == ASPAS || c == ASPASSIMPLES || c == ASTERISCO ||
           c == SOMA || c == SUBTRACAO;
}

int ehEspaco(char c){
	return c == ESPACO;
}
