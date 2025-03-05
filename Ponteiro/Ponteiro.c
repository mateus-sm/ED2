#include <stdio.h>
#include <stdlib.h>

typedef struct caixa {
    int info;
    struct caixa *prox;
} Caixa;

void inserir(Caixa **inicio, int info);
void exibir(Caixa **inicio);

int main(void) {
    Caixa *inicio = NULL;

    inserir(&inicio, 10);
    inserir(&inicio, 15);
    inserir(&inicio, 30);
    exibir(&inicio);

    getchar();
    return 0;
}

void inserir(Caixa **inicio, int info) {
    Caixa *aux, *nova;

    nova = (Caixa *)malloc(sizeof(Caixa));
    nova->info = info;
    nova->prox = NULL;

    if (*inicio == NULL) {
        *inicio = nova;
    } else {
        aux = *inicio;
        while(aux->prox != NULL) {
            aux = aux->prox;
        }

        aux->prox = nova;
    }
}

void exibir(Caixa **inicio) {
    Caixa *aux;
    aux = *inicio;

    if (*inicio != NULL) {
        while(aux->prox != NULL) {
            printf("%d -> ", aux->info);
            aux = aux->prox;
        }

        printf("%d", aux->info);
        printf("\n");
    }
}