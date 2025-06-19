#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct reg_lista {
    struct gen *cabeca;
    struct gen *calda;
};

union info_lista {
    char info[8];
    struct reg_lista lista;
};

struct gen {
    int terminal;
    union info_lista no;
};

typedef struct gen Gen;

#include "pilha.h"

char nula(Gen *l) {
    return l == NULL;
}

char atomo(Gen *l) {
    return !nula(l) && l->terminal;
}

Gen* Head(Gen *l) {
    if (!atomo(l) && !nula(l)) {
        return l->no.lista.cabeca;
    }
}

Gen* Tail(Gen *l) {
    if (!atomo(l) && !nula(l)) {
        return l->no.lista.calda;
    }
}

Gen* criaT(char *info) {
    Gen* l = (Gen*)malloc(sizeof(Gen));
    l->terminal = 1;
    strcpy(l->no.info, info);
    return l;
}

Gen* cons(Gen* H, Gen* T) {
    if (!atomo(T)) {
        Gen *l = (Gen*)malloc(sizeof(Gen));
        l->terminal = 0;
        l->no.lista.cabeca = H;
        l->no.lista.calda = T;
        return l;
    }
}

void exibir(Gen* l) {
    Pilha P;
    inicializaPilha(&P);

    printf("[");
    while (l != NULL || !pilhaVazia(&P)) {
        if (l == NULL) {
            printf("]");
            pop(&P, &l);
            l = Tail(l);
        } else {
            if (!atomo(l)) {
                push(&P, l);
                l = Head(l);
                if (!atomo(l)) { printf("["); }
            } else {
                printf("%s", l->no.info);
                pop(&P, &l);
                l = Tail(l);
                if (!nula(l)) { printf(", "); }
            }
        }
    }
    printf("]");
}

void percorrerGen(Gen* l) {
    Pilha P;
    inicializaPilha(&P);

    while (l != NULL || !pilhaVazia(&P)) {
        if (l == NULL) { //Quando um tail der NULL
            pop(&P, &l);
            l = Tail(l);
        } else {
            if (!atomo(l)) { // Caixa comum
                push(&P, l);
                l = Head(l);
            } else { // Quando chegar em um atomo
                pop(&P, &l);
                l = Tail(l);
            }
        }
    }
}

//1:-)  Faça  um  algoritmo  que  receba  uma  Lista  Generalizada  L  por  parâmetro  e  a  retorne 
//ordenando  todos  os  átomos  de  cada  uma  de  suas  sublistas.  O  algoritmo  deve  ordenar  os 
//átomos em cada Lista Generalizada (sublista). 
//Estrategia:
//Criar função para ordenar a partir de uma cabeça de lista
//Ordenar apenas aquele nivel
//Criar função que recupera as cabeças de lista relevantes para serem utilizadas
//  na função anterior



int main(void) {
    Gen *L = NULL;

    L = (Gen*)malloc(sizeof(Gen));
    L = cons(criaT("aaa"), cons(criaT("bbb"), cons(cons(criaT("ccc"), NULL), NULL)));
    exibir(L);
    puts("");

    system("pause");
    return 0;
}

