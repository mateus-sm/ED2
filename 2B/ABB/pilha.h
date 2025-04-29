#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da pilha
#define MAX 25

typedef struct pilha {
    int topo;
    Tree *pilha[MAX];
} Pilha;

//Protótipos das funções
void inicializaPilha(Pilha *P);
int pilhaCheia(Pilha *P);
int pilhaVazia(Pilha *P);
void push(Pilha *P, Tree *elemento);
void pop(Pilha *P, Tree **elemento);
Tree* topo(Pilha *P);
void exibirPilha(Pilha *P);

void inicializaPilha(Pilha *P) {
    P->topo = -1;
}

int pilhaCheia(Pilha *P) {
    return (P->topo == MAX - 1);
}

int pilhaVazia(Pilha *P) {
    return (P->topo == -1);
}

void push(Pilha *P, Tree *elemento) {
    if (!pilhaCheia(P)) {
        P->pilha[++P->topo] = elemento;
    } else {
        printf("Erro: Pilha cheia!\n");
    }
}

void pop(Pilha *P, Tree **elemento) {
    if (!pilhaVazia(P)) {
        *elemento = P->pilha[P->topo--];
    } else {
        *elemento = NULL; // Indica pilha vazia
    }
}

Tree* topo(Pilha *P) {
    if (!pilhaVazia(P)) {
        return P->pilha[P->topo];
    }
    return NULL;
}

void exibirPilha(Pilha *P) {
    Pilha temp;
    Tree *elem;
    
    inicializaPilha(&temp);
    
    printf("\nConteúdo da Pilha:");
    while (!pilhaVazia(P)) {
        pop(P, &elem);
        if (elem->info) {
            printf("\n [INFO] %s", elem->info);
        } else {
            printf("\n [Lista] Endereço: %p", (void*)elem);
        }
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
    Tree *aux;

    while (!pilhaVazia(&P)) {
        i++;
        pop(&P, &aux);
    }

    return i;
}