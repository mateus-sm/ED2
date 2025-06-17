#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50

typedef struct fila {
    int ini, fim;
    Tree *fila[MAX];
} Fila;

// Protótipos
void inicializaFila(Fila *F);
int filaVazia(Fila *F);
int filaCheia(Fila *F);
void enqueue(Fila *F, Tree *elemento);
void dequeue(Fila *F, Tree **elemento);
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

void enqueue(Fila *F, Tree *elemento) {
    if (!filaCheia(F)) {
        F->fila[F->fim] = elemento;
        F->fim = (F->fim + 1) % MAX;
    } else {
        printf("Erro: Fila cheia!\n");
    }
}

void dequeue(Fila *F, Tree **elemento) {
    if (!filaVazia(F)) {
        *elemento = F->fila[F->ini];
        F->ini = (F->ini + 1) % MAX;
    } else {
        *elemento = NULL;
    }
}

void exibirFila(Fila *F) {
    Fila temp;
    Tree *elem;

    inicializaFila(&temp);
    printf("\nConteúdo da Fila:");
    while (!filaVazia(F)) {
        dequeue(F, &elem);
        if (elem && elem->info) {
            printf("\n [INFO] %s", elem->info);
        } else {
            printf("\n [Lista] Endereço: %p", (void *)elem);
        }
        enqueue(&temp, elem);
    }

    // Restaura a fila original
    while (!filaVazia(&temp)) {
        dequeue(&temp, &elem);
        enqueue(F, elem);
    }
}

int tamanhoFila(Fila F) {
    int cont = 0;
    Tree *aux;
    while (!filaVazia(&F)) {
        dequeue(&F, &aux);
        cont++;
    }
    return cont;
}
