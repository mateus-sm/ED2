#include <stdio.h>
#include <stdlib.h>

struct tree {
    int info;
    struct tree *esq, *dir;
};

typedef struct tree Tree;

#include "pilha.h"

//Inicializar uma árvore binária;
void inicializaArv(Tree **t) {
    *t = NULL;
}

//Criar um nó
Tree *criaNoArv(int info) {
    Tree *aux;

    aux = (Tree*)malloc(sizeof(Tree));
    aux->info = info;
    aux->dir = NULL;
    aux->esq = NULL;
 
    return aux;
}

//Verificar se a árvore está vazia ou não
char arvVazia(Tree *t) {
    return t == NULL;
}

//Precisa percorrer ela inteira
char localizaNo(Tree *t, int alvo, Tree **arg) {
    Tree *aux = t;
    Pilha P;
    inicializaPilha(&P);

    if(!arvVazia(aux)) {
        push(&P, aux);
        while(!pilhaVazia(&P)) {
            //Se chegou em um fim, volta para ultimo endereço que tem caminho a direita
            if (aux == NULL) {
                pop(&P, &aux);
            }
            //Se nao está no fim então precisa ser avaliado e processado
            else {
                if (aux->info == alvo) {
                    *arg = aux;
                }
                if (aux->dir != NULL) {
                    push(&P, aux->dir);
                }
                aux = aux->esq;
            }
        }
    }

    return (arg == NULL) ? 0 : 1;
}

// Criar um filho à direita de um dado nó; 
// Criar um filho à esquerda de um dado nó;
// Precisa percorrer ela toda, procurando o pai do nó a ser 
//  criado(dunção LocalizaNo()), e ai sim criar o nó
void insereArv(Tree **t, int info, int pai_info, char dir_info) {
    Tree *pai = NULL;

    if (arvVazia(*t)) {
        *t = criaNoArv(info);
    } else {
        localizaNo(*t, pai_info, &pai);

        if(pai->info == pai_info) {
            if (dir_info == 'D' && pai->dir == NULL) {
                pai->dir = criaNoArv(info);
            } 
            else {
                if (dir_info == 'E' && pai->esq == NULL) {
                    pai->esq = criaNoArv(info);
                }
            }
        }
    }
}

//Arvore comum, precisa percorrer ela por inteiro
//  Só funciona para elementos pra esquerda
int nivel(Tree *t, int info) {
    Pilha P;
    int i = 0;

    inicializaPilha(&P);

    if (t != NULL) {
        push(&P, t);
        while(!pilhaVazia(&P)) {
            if (t == NULL) { //Cheguei no max esquerda
                pop(&P, &t); //Volto para o pai
                t = t->dir; //Vou para a direita
            }
            else {
                push(&P, t);
                if (t->info == info) {
                    i = tamanhoPilha(P);
                }
                t = t->esq;
            }
        }
    }

    return i;
}

void vazia(Tree **t) {
    Tree *aux;
    Pilha P;

    inicializaPilha(&P);

    if (*t != NULL) {
        push(&P, *t);
        while(!pilhaVazia(&P)) {
            if (*t == NULL) {
                pop(&P, &*t);
                aux = *t;
                *t = (*t)->dir; 
                free(aux);
            }
            else {
                push(&P, *t);
                *t = (*t)->esq;
            }
        }
    }

    *t = NULL;
}

// P1
// 
// 
//
//
//214
//200

//6308846
int main() {
    Tree *t;

    inicializaArv(&t);
    //printf("%d\n", arvVazia(t));
    
    insereArv(&t, 10, 0, ' ');
    printf("%d\n", t->info);
    
    insereArv(&t, 20, 10, 'E');
    // printf("Esq = %p\n", t->esq);
    // printf("Dir = %p\n", t->dir);
    printf("%d\n", t->esq->info);

    insereArv(&t, 30, 20, 'E');
    printf("%d\n", t->esq->esq->info);
    insereArv(&t, 40, 30, 'E');
    insereArv(&t, 50, 20, 'D');
    insereArv(&t, 60, 50, 'D');
    insereArv(&t, 99, 40, 'D');
    insereArv(&t, 65, 10, 'D');
    insereArv(&t, 70, 65, 'D');
    printf("%d\n", nivel(t, 50));
    printf("%d\n", nivel(t, 60));
    printf("%d\n", nivel(t, 99));
    printf("%d\n", nivel(t, 70));

    system("pause");
    return 0;
}