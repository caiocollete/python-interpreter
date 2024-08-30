#define MAX_LINHA 100
#define MAX_TOKEN 100


typedef struct {
	char token[MAX_TOKEN];
	struct Token *prox;
} Token;

typedef struct {
	struct No *ant, *prox;
	Token *tk;
} No;

