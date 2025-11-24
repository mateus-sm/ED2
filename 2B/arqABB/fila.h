#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

typedef struct fila {
    int ini, fim;
    Gravar fila[MAX];
} Fila;

// Protótipos
void inicializaFila(Fila *F);
int filaVazia(Fila *F);
int filaCheia(Fila *F);
void enqueue(Fila *F, Gravar elemento);
void dequeue(Fila *F, Gravar *elemento);
void exibirFila(Fila *F);
int tamanhoFila(Fila F);

// Funções
void inicializaFila(Fila *F) {
    F->ini = F->fim = 0;
}

int filaVazia(Fila *F) {
    return (F->ini == F->fim);
}

int filaCheia(Fila *F) {
    return ((F->fim + 1) % MAX == F->ini);
}

void enqueue(Fila *F, Gravar elemento) {
    if (!filaCheia(F)) {
        F->fila[F->fim] = elemento;
        F->fim = (F->fim + 1) % MAX;
    } else {
        printf("Erro: Fila cheia!\n");
    }
}

void dequeue(Fila *F, Gravar *elemento) {
    if (!filaVazia(F)) {
        *elemento = F->fila[F->ini];
        F->ini = (F->ini + 1) % MAX;
    } else {
        elemento->id = -1;
        elemento->nome[0] = '\0';
    }
}

void exibirFila(Fila *F) {
    Fila temp;
    Gravar elem;

    inicializaFila(&temp);
    printf("\nConteúdo da Fila:");

    while (!filaVazia(F)) {
        dequeue(F, &elem);
        printf("\n ID: %d | Nome: %s", elem.id, elem.nome);
        enqueue(&temp, elem);
    }

    while (!filaVazia(&temp)) {
        dequeue(&temp, &elem);
        enqueue(F, elem);
    }
}

int tamanhoFila(Fila F) {
    int cont = 0;
    Gravar aux;

    while (!filaVazia(&F)) {
        dequeue(&F, &aux);
        cont++;
    }
    return cont;
}
