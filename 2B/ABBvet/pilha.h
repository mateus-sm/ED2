#include <stdio.h>
#include <stdlib.h>

#define MAX 50

// Estrutura da pilha de inteiros
typedef struct pilha {
    int topo;
    int pilha[MAX];
} Pilha;

//Protótipos das funções
void inicializaPilha(Pilha *P);
int pilhaCheia(Pilha *P);
int pilhaVazia(Pilha *P);
void push(Pilha *P, int elemento);
int pop(Pilha *P, int *elemento);
int topo(Pilha *P);
void exibirPilha(Pilha *P);
int tamanhoPilha(Pilha P);

// ------------------------ IMPLEMENTAÇÕES ------------------------

void inicializaPilha(Pilha *P) {
    P->topo = -1;
}

int pilhaCheia(Pilha *P) {
    return (P->topo == MAX - 1);
}

int pilhaVazia(Pilha *P) {
    return (P->topo == -1);
}

void push(Pilha *P, int elemento) {
    if (!pilhaCheia(P)) {
        P->pilha[++P->topo] = elemento;
    } else {
        printf("Erro: Pilha cheia!\n");
    }
}

int pop(Pilha *P, int *elemento) {
    if (!pilhaVazia(P)) {
        *elemento = P->pilha[P->topo--];
        return 1;
    } else {
        *elemento = -1;  // apenas um valor padrão para indicar erro
        return 0;
    }
}

int topo(Pilha *P) {
    if (!pilhaVazia(P)) {
        return P->pilha[P->topo];
    }
    return -1; // pilha vazia
}

void exibirPilha(Pilha *P) {
    Pilha temp;
    int elem;

    inicializaPilha(&temp);

    printf("\nConteúdo da Pilha:");
    while (!pilhaVazia(P)) {
        pop(P, &elem);
        printf("\n %d", elem);
        push(&temp, elem);
    }

    // Restaura a pilha original
    while (!pilhaVazia(&temp)) {
        pop(&temp, &elem);
        push(P, elem);
    }
}

int tamanhoPilha(Pilha P) {
    int i = 0;
    int aux;

    while (!pilhaVazia(&P)) {
        pop(&P, &aux);
        i++;
    }

    return i;
}
