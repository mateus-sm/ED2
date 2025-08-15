#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura da pilha
#define MAX 25

typedef struct pilha {
    int topo;
    Exp *pilha[MAX];  // Armazena ponteiros para Exp
} Pilha;

// --- Protótipos das funções ---

// Inicializa a pilha
void inicializaPilha(Pilha *P);

// Verifica se a pilha está cheia
int pilhaCheia(Pilha *P);

// Verifica se a pilha está vazia
int pilhaVazia(Pilha *P);

// Empilha um elemento (ponteiro para Exp)
void push(Pilha *P, Exp *elemento);

// Desempilha um elemento (recebe um ponteiro para Exp *)
void pop(Pilha *P, Exp **elemento);

// Retorna o elemento no topo (sem remover)
Exp * topo(Pilha *P);

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

void push(Pilha *P, Exp *elemento) {
    if (!pilhaCheia(P)) {
        P->pilha[++P->topo] = elemento;
    } else {
        printf("Erro: Pilha cheia!\n");
    }
}

void pop(Pilha *P, Exp **elemento) {
    if (!pilhaVazia(P)) {
        *elemento = P->pilha[P->topo--];
    } else {
        *elemento = NULL; // Indica pilha vazia
    }
}

Exp * topo(Pilha *P) {
    if (!pilhaVazia(P)) {
        return P->pilha[P->topo];
    }
    return NULL;
}

// void exibirPilha(Pilha *P) {
//     Pilha temp;
//     Exp *elem;

//     inicializaPilha(&temp);

//     printf("\nConteúdo da Pilha:");
//     while (!pilhaVazia(P)) {
//         pop(P, &elem);
//         if (elem->terminal ) {       AJUSTAR PARA A ESTRUTURA!!
//             printf("\n [Atomo] %s", elem->no.info);
//         } else {
//             printf("\n [Lista] Endereço: %p", (void*)elem);
//         }
//         push(&temp, elem);
//     }
    
//     // Restaura a pilha original
//     while (!pilhaVazia(&temp)) {
//         pop(&temp, &elem);
//         push(P, elem);
//     }
// }