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

// Insere na árvore binária de busca
// Checar os valores, inserir de acordo com tamanho
void insereArv(Tree **t, int info) {
    Tree *aux = *t, *ant = NULL;
    Pilha P;
    inicializaPilha(&P);

    if (*t == NULL) {
        *t = criaNoArv(info);
    } else {
        while(aux != NULL) {
            ant = aux;
            aux = (info > aux->info) ? aux->dir : aux->esq;
        }

        if (info > ant->info) {
            ant->dir = criaNoArv(info);
        } else {
            ant->esq = criaNoArv(info);
        }
    }
}

void insereArvRecusrsivo(Tree **t, int info) {
    if(*t == NULL) {
        *t = criaNoArv(info);
    } else {
        if (info > (*t)->info) {
            insereArvRecusrsivo(&(*t)->dir, info);
        } else {
            insereArvRecusrsivo(&(*t)->esq, info);
        }
    }
}

//Verificar qual o nível (ou profundidade) de um dado nó.
// Arvore ABB, NÃO precisa percorrer ela por inteiro
int nivel(Tree *t, int info) {
    Tree *aux = t;
    Pilha P;
    int nivel = 1;

    while(aux != NULL && aux->info != info) {
        aux = (info > aux->info) ? aux->dir : aux->esq;
        nivel++;
    }

    return nivel;
}

//Tornar uma árvore Vazia (free em todos os nodos da árvore). 
//Estratégia: Percorrer inteira (Pré Ordem)
//Razão: precisa-se excluir todos os nós
void vazia(Tree **t) {
    Tree *aux;
    Pilha P;

    inicializaPilha(&P);

    if (*t != NULL || !pilhaVazia(&P)) {
        if (*t == NULL) {
            pop(&P, &*t);
            aux = *t;
            *t = (*t)->dir;
            free(aux);
        } else {
            push(&P, *t);
            *t = (*t)->esq;
        }
    }

    *t = NULL;
}

void Order1(Tree *t) {
    Tree *raiz = t;
    Pilha P;

    inicializaPilha(&P);

    if (raiz != NULL) {
        while(raiz != NULL || !pilhaVazia(&P)) {
            if (raiz == NULL) { //Cheguei no max esquerda
                pop(&P, &raiz); //Volto para o pai
                printf("%d ", raiz->info); //In Ordem
                raiz = raiz->dir; //Vou para a direita
            }
            else {
                //printf("%d ", raiz->info); //Pré Ordem
                push(&P, raiz);
                raiz = raiz->esq;
            }
        }
    }

    puts("");
}

void Order2(Tree *t) {
    Tree *raiz = t;
    Pilha P;

    inicializaPilha(&P);

    if (raiz != NULL) {
        push(&P, raiz);
        while(!pilhaVazia(&P)) {
            if (raiz != NULL) { //max esquerda + 1 direita
                pop(&P, &raiz);

                while(raiz != NULL) { // ir para max esquerda
                    //printf("%d ", raiz->info); //Pré Ordem
                    push(&P, raiz);
                    raiz = raiz->esq;
                }
            }

            pop(&P, &raiz);
            printf("%d ", raiz->info); //In Ordem
            raiz = raiz->dir; //ir para direita
            if(raiz != NULL) {
                push(&P, raiz); //Se direita existe guarda
            }
        }
    }

    puts("");
}

void Order3(Tree *t) {
    Tree *raiz = NULL;
    Pilha P1, P2;
    inicializaPilha(&P1);
    inicializaPilha(&P2);

    push(&P1, t);
    while(!pilhaVazia(&P1)) {
        pop(&P1, &raiz);
        
        if (raiz != NULL) {
            push(&P2, raiz); //Pós Ordem 1° Esq 2° Dir
            //printf("%d ", raiz->info); //Pré Ordem 1° Dir 2° Esq
            push(&P1, raiz->esq);
            push(&P1, raiz->dir);
        }
    }

    printf("Pos Ordem: ");
    while(!pilhaVazia(&P2)) {
        pop(&P2, &raiz);
        printf("%d ", raiz->info);
    }
}

//Retornar o pai de um dado nó.
//Estratégia: toda vez que for pra esq ou dir verificar o atual com o prox
//Percorrer arvore por completo pois não há ordenação
Tree *Pai(Tree *t, int info) {
    Tree *raiz = t, *aux = NULL, *pai = NULL;
    Pilha P;

    inicializaPilha(&P);

    while(raiz != NULL || !pilhaVazia(&P)) {
        if (raiz == NULL) {
            pop(&P, &raiz);
            aux = raiz;
            raiz = raiz->dir;
            if (raiz != NULL && raiz->info == info) {
                pai = aux;
            }
        } else {
            push(&P, raiz);
            aux = raiz;
            raiz = raiz->esq;
            if (raiz != NULL && raiz->info == info) {
                pai = aux;
            }
        }
    }

    return pai;
}


//1)  Dado  um  elemento  verificar  se  está  cadastrado  na  árvore  e,  se  estiver,  
//        exibir  seus sucessores.
void antecessores(Tree *t, int info) {
    Tree *aux = t, *no = NULL;
    Pilha P1, P2;
    inicializaPilha(&P1);
    inicializaPilha(&P2);

    while(aux != NULL && aux->info != info) {
        push(&P1, aux);
        aux = (info > aux->info) ? aux->dir : aux->esq;
    }
    push(&P1, aux);

    if (aux != NULL && aux->info == info) {
        if (!pilhaVazia(&P1)) {
            while(!pilhaVazia(&P1)) {
                pop(&P1, &no);
                push(&P2, no);
            }
            while (!pilhaVazia(&P2)) {
                pop(&P2, &no);
                printf("%d - ", no->info);
            }
            printf("%s", "NULL\n");
        }
    }
}

void exibirABB(Tree *arv, int *n) {
    if (arv != NULL) {
        if((*n) == -1) {
            printf("Arvore: \n");
        }
        (*n)++;
        exibirABB(arv->dir, n);
        for(int i = 0; i < 5 * (*n); i++) {printf(" ");}
        printf("(%d)\n", arv->info);
        exibirABB(arv->esq, n);
        (*n)--;
    }
}

int main() {
    Tree *t = NULL, *pai = NULL;
    int n;

    inicializaArv(&t);
    //printf("%d\n", arvVazia(t));
    
    insereArvRecusrsivo(&t, 5);
    printf("%d\n", t->info);
    
    insereArvRecusrsivo(&t, 3);
    //printf("Esq = %p\n", t->esq);
    //printf("Dir = %p\n", t->dir);
    printf("%d\n", t->esq->info);

    insereArvRecusrsivo(&t, 2);
    printf("%d\n", t->esq->esq->info);
    insereArvRecusrsivo(&t, 4);
    insereArvRecusrsivo(&t, 8);
    insereArvRecusrsivo(&t, 9);
    
    // printf("%d\n", nivel(t, 5));
    // printf("%d\n", nivel(t, 8));
    // printf("%d\n", nivel(t, 4));
    // printf("%d\n", nivel(t, 2));
    n = -1;
    exibirABB(t, &n);

    printf("Percorrer: \n");
    Order1(t);
    Order2(t);
    Order3(t);

    printf("\nAntecessores: \n");
    antecessores(t, 2);
    antecessores(t, 4);
    antecessores(t, 9);

    // pai = Pai(t, 40);
    // printf("%d\n", pai->info);

    system("pause");
    return 0;
}