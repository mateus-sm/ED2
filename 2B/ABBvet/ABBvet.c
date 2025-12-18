#include <stdio.h>
#include "pilha.h"

#define max 100

void inicializa(int *abb) {
    for (int i = 0; i < max; i ++) {
        abb[i] = -1;
    }
}

void nivel_vet(int abb[]) {
    int pos = 0;
    Pilha p;

    inicializaPilha(&p);

    push(&p, 0);
    while (!pilhaVazia(&p)) {
        pop(&p, &pos);

        if (abb[pos * 2 + 1] != -1)
            push(&p,pos * 2 + 1);
        if (abb[pos * 2 + 2] != -1)
            push(&p, pos * 2 + 2);
    }

}

void pre_ordem_vet(int abb[]) {
    int pos = 0;
    Pilha p;
    inicializaPilha(&p);

    while (abb[pos] != -1 || !pilhaVazia(&p)) {
        if (abb[pos] == -1) {
            pop(&p, &pos);
            pos = pos * 2 + 2;
        } else {
            push(&p, pos);
            printf("%d", abb[pos]);
            pos = pos * 2 + 1;
        }
    }
}

void in_ordem_vet(int abb[]) {
    int pos = 0;
    Pilha p;

    inicializaPilha(&p);

    while (abb[pos] != -1 || !pilhaVazia(&p)) {
        if (abb[pos] == -1) {
            pop(&p, &pos);
            printf("%d", abb[pos]);
            pos = pos * 2 + 2;
        } else {
            push(&p, pos);
            pos = pos * 2 + 1;
        }
    }
}

void pos_ordem(int abb[]) {
    int pos = 0, nivel = 1;
    Pilha p,p2, pn, pn2;

    inicializaPilha(&p);
    inicializaPilha(&p2);
    inicializaPilha(&pn);
    inicializaPilha(&pn2);

    push(&pn, nivel);
    push(&p, pos);
    while (!pilhaVazia(&p)) {
        pop(&p, &pos);
        pop(&pn, &nivel);

        if (abb[pos] != -1) {
            push(&p2, pos);
            push(&pn2, nivel);

            if (abb[pos * 2 + 1] != -1) {
                push(&p,pos * 2 + 1);
                push(&pn, nivel + 1);
            }
            if (abb[pos * 2 + 2] != -1) {
                push(&p,pos * 2 + 2);
                push(&pn, nivel + 1);
            }
        }
    }

    while (!pilhaVazia(&p2)) {
        pop(&p2, &pos);
        pop(&pn2, &nivel);
        printf("Elem: %d, nivel: %d\n", abb[pos], nivel);
    }
}

void inseri_abb_vet(int abb[], int valor) {
    int pos = 0;

    if (abb[pos] == -1) {
        abb[pos] = valor;
    }
    else {
        while (abb[pos] != -1) {
            if (valor > abb[pos]) {
                pos = pos * 2 + 2;
            } else {
                pos = pos * 2 + 1;
            }
        }
        abb[pos] = valor;
    }
}

int main(void) {
    int abb[100];
    inicializa(abb);
    inseri_abb_vet(abb, 4);
    inseri_abb_vet(abb, 3);
    inseri_abb_vet(abb, 5);
    inseri_abb_vet(abb, 2);
    inseri_abb_vet(abb, 1);
    inseri_abb_vet(abb, 6);
    pre_ordem_vet(abb); puts("");
    in_ordem_vet(abb); puts("");
    pos_ordem(abb);
    return 0;
}