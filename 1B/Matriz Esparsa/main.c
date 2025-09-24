#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

struct celula {
	int linha, coluna, valor;
	struct celula *plin, *pcol;
};

typedef struct celula Cel;

struct ponteiro {
	int pos;
	struct ponteiro *prox;
	struct celula *prim;
};

typedef struct ponteiro Pont;

struct descritor {
    Pont *lin, *col;
};

typedef struct descritor Desc;


void init(Desc *desc) {
	desc->lin = desc->col = NULL;
}

void Posicionar(Pont **Cabeca, int xy, Pont **ret) {
	Pont *nova, *aux, *ant;

	if ((*Cabeca) == NULL) { // Não existem linhas
		nova = (Pont*)malloc(sizeof(Pont));
		nova->prox = NULL;
		nova->prim = NULL;
		nova->pos = xy;
		*Cabeca = *ret = nova;
	} 
	else {
		aux = *Cabeca;
		while (aux != NULL && aux->pos < xy) { // Ver se existe a linha
			ant = aux;
			aux = aux->prox;
		}

		if (aux != NULL && aux->pos == xy) { // Achou a linha
			*ret = aux;
		}

		else { //Cria a linha
			nova = (Pont*)malloc(sizeof(Pont));
			nova->prox = NULL;
			nova->prim = NULL;
			nova->pos = xy;

			if (aux == *Cabeca) { // Primeira posição
				nova->prox = *Cabeca;
				*ret = *Cabeca = nova;
			}
			else {
				nova->prox = aux;
				ant->prox = nova;
				*ret = nova;
			}
		}
	}
}

void VerificaOcupado(Pont *linha, int y, Cel **ret) {
	Cel *aux = linha->prim;

	//Busca
	while (aux != NULL && y > aux->coluna) {
		aux = aux->plin;
	}

	if (aux != NULL && aux->coluna == y) {
		*ret = aux;
	} else {
		*ret = NULL;
	}
}

void InsereMat(Desc *desc, int x, int y, int valor) {
	Cel *aux, *ant, *nova;
	Pont *auxLinha, *auxColuna;

	Posicionar(&desc->lin, x, &auxLinha);
	Posicionar(&desc->col, y, &auxColuna);

	// Linha
	VerificaOcupado(auxLinha, y, &aux);
	if (aux != NULL) { //Atualiza o valor
		aux->valor = valor;
	}

	else {
		nova = (Cel*)malloc(sizeof(Cel));
		nova->linha = x;
		nova->coluna = y;
		nova->valor = valor;

		//Tratar linha
		if (auxLinha->prim == NULL) { // Linha vazia, só inserir no prim
			auxLinha->prim = nova;
			nova->plin = NULL;
		}
		else {
			if (y < auxLinha->prim->coluna) { // Primeira pos
				nova->plin = auxLinha->prim;
				auxLinha->prim = nova;
			}
			else { // Meio ou fim
				ant = auxLinha->prim;
				aux = ant->plin;

				while (aux != NULL && y > aux->coluna) {
					ant = aux;
					aux = aux->plin;
				}

				nova->plin = aux;
				ant->plin = nova;
			}
		}

		//Tratar Col
		if (auxColuna->prim == NULL) { // Sem coluna
			auxColuna->prim = nova;
			nova->pcol = NULL;
		}
		else {
			if (x < auxColuna->prim->linha) {
				nova->pcol = auxColuna->prim;
				auxColuna->prim = nova;
			}
			else {
				ant = auxColuna->prim;
				aux = ant->pcol;

				while (aux != NULL && x > aux->linha) {
					ant = aux;
					aux = aux->pcol;
				}

				nova->pcol = aux;
				ant->pcol = nova;
			}
		}
	}
}

void exibeMat(Desc *desc, int x, int y) {
    Pont *Linha = desc->lin;
	Cel *atual;

    while (Linha != NULL) {
        atual = Linha->prim;

        for (int i = 0; i < y; i++) {
            if (atual != NULL && atual->coluna == i) {
                printf("%d ", atual->valor);
                atual = atual->plin;
            } else {
                printf("0 ");
            }
        }
        printf("\n");

        Linha = Linha->prox;
    }
}


int main(void) {
	Desc desc;
	init(&desc);

	InsereMat(&desc, 0, 0, 1);
	InsereMat(&desc, 1, 1, 1);
	InsereMat(&desc, 2, 2, 1);
	InsereMat(&desc, 3, 3, 1);

	exibeMat(&desc, 4, 4);
	system("pause");
	return 0;
}
