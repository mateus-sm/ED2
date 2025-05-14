#include <stdio.h>

// Estrutura da pilha
#define MAX 100

typedef struct pilha {
    int topo;
    Huff *pilha[MAX];
} Pilha;

//Protótipos das funções
void inicializaPilha(Pilha *P);
int pilhaCheia(Pilha P);
int pilhaVazia(Pilha P);
void push(Pilha *P, Huff *elemento);
void pop(Pilha *P, Huff **elemento);
Huff* topo(Pilha P);
void exibirPilha(Pilha P);

//Funções
void inicializaPilha(Pilha *P) {
    P->topo = -1;
}

int pilhaCheia(Pilha P) {
    return (P.topo == MAX - 1);
}

int pilhaVazia(Pilha P) {
    return (P.topo == -1);
}

void push(Pilha *P, Huff *elemento) {
    if (!pilhaCheia((*P))) {
        P->pilha[++P->topo] = elemento;
    } else {
        printf("Pilha cheia!\n");
    }
}

void pop(Pilha *P, Huff **elemento) {
    if (!pilhaVazia((*P))) {
        *elemento = P->pilha[P->topo--];
    } else {
        *elemento = NULL;
    }
}

Huff* topo(Pilha P) {
    if (!pilhaVazia(P)) {
        return P.pilha[P.topo];
    }
    return NULL;
}

void exibirPilha(Pilha P) {
    Huff *elem;
    
    printf("Conteúdo da Pilha:\n");
    while (!pilhaVazia(P)) {
        pop(&P, &elem);

        if (elem->simbolo) {
            printf(" [Simbolo] %s\n", elem->simbolo);
        } else {
            printf(" [Lista] Endereço: %p\n", (void*)elem);
        }
    }
}

int tamanhoPilha(Pilha P) {
    int i = 0;
    Huff *aux;

    while (!pilhaVazia(P)) {
        i++;
        pop(&P, &aux);
    }

    return i;
}