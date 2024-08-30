void inicializar(No **lista);
void inicializar_com_null(No **lista);

void inicializar_com_null(No* *lista) {
	*lista = NULL;
}

void inicializar(No* *lista) {
	*lista = (No*)malloc(sizeof(No));
	(*lista)->ant = (*lista)->prox = (*lista)->tk = NULL;
}
