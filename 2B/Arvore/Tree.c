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

Tree *localizaNo(Tree *t, int alvo) {
    Tree *no = NULL, *aux = t;
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
                    no = aux;
                }
                if (aux->dir != NULL) {
                    push(&P, aux->dir);
                }
                aux = aux->esq;
            }
        }
    }

    return no;
}

// Criar um filho à direita de um dado nó; 
// Criar um filho à esquerda de um dado nó;
void insereArv(Tree **t, int info, int pai_info, char dir_info) {
    Tree *pai = NULL;

    if (arvVazia(*t)) {
        *t = criaNoArv(info);
    } else {
        pai = localizaNo(*t, pai_info);

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

int profundidade(Tree *t, int info) {
    Pilha P;
    int i = 0, sub = 0;

    inicializaPilha(&P);
    push(&P, t->dir);
    while(!pilhaVazia(&P)) {
        if (t == NULL) {
            pop(&P, &t);
            i = i - sub;
            sub = 0;
        }
        else {
            if (t->info == info) {
                inicializaPilha(&P);
            }
            if (t->dir != NULL) {
                push(&P, t->dir);
                sub = 0;
            } else {
                sub++;
            }
            t = t->esq;
            i++;
        }
    }

    return i;
}

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
    insereArv(&t, 99, 40, 'D');
    printf("%d\n", profundidade(t, 50));

    system("pause");
    return 0;
}