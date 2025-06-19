#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da pilha
#define MAX 25

typedef struct pilha {
    int topo;
    Gen *pilha[MAX];  // Armazena ponteiros para Gen
} Pilha;

// --- Protótipos das funções ---

// Inicializa a pilha
void inicializaPilha(Pilha *P);

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *P);

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *P);

// Empilha um elemento (ponteiro para Gen)
void push(Pilha *P, Gen *elemento);

// Desempilha um elemento (recebe um ponteiro para Gen *)
void pop(Pilha *P, Gen **elemento);

// Retorna o elemento no topo (sem remover)
Gen * topo(Pilha *P);

// Exibe a pilha (para debug)
void exibirPilha(Pilha *P);

// --- Implementações (inline para header-only) ---

void inicializaPilha(Pilha *P) {
    P->topo = -1;
}

int pilhaCheia(Pilha *P) {
    return (P->topo == MAX - 1);
}

int pilhaVazia(Pilha *P) {
    return (P->topo == -1);
}

void push(Pilha *P, Gen *elemento) {
    if (!pilhaCheia(P)) {
        P->pilha[++P->topo] = elemento;
    } else {
        printf("Erro: Pilha cheia!\n");
    }
}

void pop(Pilha *P, Gen **elemento) {
    if (!pilhaVazia(P)) {
        *elemento = P->pilha[P->topo--];
    } else {
        *elemento = NULL; // Indica pilha vazia
    }
}

Gen * topo(Pilha *P) {
    if (!pilhaVazia(P)) {
        return P->pilha[P->topo];
    }
    return NULL;
}

void exibirPilha(Pilha *P) {
    Pilha temp;
    Gen *elem;

    inicializaPilha(&temp);

    printf("\nConteúdo da Pilha:");
    while (!pilhaVazia(P)) {
        pop(P, &elem);
        if (elem->terminal) {
            printf("\n [Atomo] %s", elem->no.info);
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