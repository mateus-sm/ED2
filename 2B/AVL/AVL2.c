#include <stdio.h>
#include <stdlib.h>

#define MAX 100

struct tree {
    double info;
    struct tree *esq, *dir;
};

typedef struct tree Tree;

#include "pilha.h"
#include "fila.h"

//Inicializar uma árvore binária;
void inicializaArv(Tree **t) {
    *t = NULL;
}

//Criar um nó
Tree *criaNoArv(double info) {
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
void insereArv(Tree **t, double info) {
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

void inserirArv(Tree **t, double info) {
    if(*t == NULL) {
        *t = criaNoArv(info);
    } else {
        if (info > (*t)->info) {
            inserirArv(&(*t)->dir, info);
        } else {
            inserirArv(&(*t)->esq, info);
        }
    }
}

//Verificar qual o nível (ou profundidade) de um dado nó.
// Arvore ABB, NÃO precisa percorrer ela por inteiro
int nivel(Tree *t, double info) {
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
Tree *Pai(Tree *t, double info) {
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

void exibir(Tree *arv, int *n) {
    if (arv != NULL) {
        if((*n) == -1) {
            printf("Arvore: \n");
        }
        (*n)++;
        exibir(arv->dir, n);
        for(int i = 0; i < 5 * (*n); i++) {printf(" ");}
        printf("(%.2lf)\n", arv->info);
        exibir(arv->esq, n);
        (*n)--;
    }
}

void busca(Tree *raiz, double info, Tree **e, Tree **pai) {
    *e = *pai = raiz;
    while(*e != NULL && (*e)->info != info) {
        *pai = *e;
        if(info < (*e)->info) {
            *e = (*e)->esq;
        } else {
            *e = (*e)->dir;
        }
    }
}

void quantNo(Tree *t, int *qtd) {
    Tree *atual = t;
    Pilha P;
    inicializaPilha(&P);

    while (atual != NULL || !pilhaVazia(&P)) {
        if (atual == NULL) {
            pop(&P, &atual);
            (*qtd)++;
            atual = atual->dir;
        } else {
            push(&P, atual);
            atual = atual->esq;
        }
    }
}

void altura(Tree *raiz, int nivel, int *maior) {
    if (raiz != NULL) {
        if (nivel > *maior) {
            *maior = nivel;
        }
        altura(raiz->dir, nivel + 1, &*maior);
        altura(raiz->esq, nivel + 1, &*maior);
    }
}

int alturaAVL(Tree *raiz) {
    int maior = 0;
    altura(raiz, 1, &maior);
    return maior;
}

void rotacaoEsquerda(Tree **p) {
    Tree *q, *temp;

    q = (*p)->dir;
    temp = q->esq;

    q->esq = *p;
    (*p)->dir = temp;
    (*p) = q;
}

void rotacaoDireita(Tree **p) {
    Tree *q, *temp;

    q = (*p)->esq;
    temp = q->dir;

    q->dir = *p;
    (*p)->esq = temp;
    (*p) = q;
}

void inserirAVL(Tree **raiz, double info, char *flag) {
    int fb, fbfilho;

    if(*raiz == NULL) {
        *raiz = criaNoArv(info);
        *flag = 0;
    } else {
        if (info < (*raiz)->info) {
            //printf("%p\n", (*raiz)->esq);
            inserirAVL(&(*raiz)->esq, info, &*flag);
        } else {
            inserirAVL(&(*raiz)->dir, info, &*flag);
        }

        if(!*flag) {
            fb = alturaAVL((*raiz)->dir) - alturaAVL((*raiz)->esq);
            if (fb == 2 || fb == -2) {
                *flag = 1;

                if (fb == 2) {
                    fbfilho = alturaAVL((*raiz)->dir->dir) - alturaAVL((*raiz)->dir->esq);

                    if (fbfilho == 1) {
                        rotacaoEsquerda(&*raiz);
                    } else {
                        rotacaoDireita(&(*raiz)->dir);
                        rotacaoEsquerda(&*raiz);
                    }
                } else {
                    fbfilho = alturaAVL((*raiz)->esq->dir) - alturaAVL((*raiz)->esq->esq);

                    if (fbfilho == -1) {
                        rotacaoDireita(&*raiz);
                    } else {
                        rotacaoEsquerda(&(*raiz)->esq);
                        rotacaoDireita(&*raiz);
                    }
                }
            }
        }
    }
}

void inserirIterativo(Tree **raiz, int valor) {
    int rotacionou = 0, FB, FBfilho;
    Tree *aux, *ant;
    Pilha p;
    inicializaPilha(&p);

    if (*raiz == NULL) {
        *raiz = criaNoArv(valor);
    } else {
        //Acho o local que vai entrar o no novo
        aux = *raiz;
        ant = aux;
        while (aux != NULL) {
            ant = aux;
            push(&p, aux);
            if (valor < aux->info) {
                aux = aux->esq;
            } else {
                aux = aux->dir;
            }
        }

        //Crio o no
        if (valor > ant->info) {
            ant->dir = criaNoArv(valor);
        } else {
            ant->esq = criaNoArv(valor);
        }

        //Enquanto eu tenho elementos e a pilha nao rotacionou
        while (!pilhaVazia(&p) && !rotacionou) {
            pop(&p, &aux);
            FB = alturaAVL(aux->dir) - alturaAVL(aux->esq);

            if (FB == 2 || FB == -2) {
                rotacionou = 1;

                if (FB == 2) {
                    FBfilho = alturaAVL(aux->dir->dir) - alturaAVL(aux->dir->esq);

                    //(2)(1)
                    if (FBfilho == 1) {
                        rotacaoEsquerda(&aux);
                    } else { // (2) (-1)
                        rotacaoDireita(&(aux->dir));
                        rotacaoEsquerda(&aux);
                    }
                }
                else {
                    FBfilho = alturaAVL(aux->esq->dir) - alturaAVL(aux->esq->esq);

                    //(-2)(-1)
                    if (FBfilho == -1) {
                        rotacaoDireita(&aux);
                    } else { //(-2)(1)
                        rotacaoEsquerda(&(aux->esq));
                        rotacaoDireita(&aux);
                    }
                }

                //Verificar estado da pilha após rotacionar
                if (!pilhaVazia(&p)) {
                    pop(&p, &ant);

                    if (aux->info > ant->info) {
                        ant->dir = aux;
                    } else {
                        ant->esq = aux;
                    }
                } else {
                    *raiz = aux;
                }
            }
        }
    }
}

int main() {
    Tree *t = NULL, *pai = NULL, *e = NULL;
    int n;
    char flag = 0;

    inicializaArv(&t);

    inserirIterativo(&t, 5);
    inserirIterativo(&t, 3);
    inserirIterativo(&t, 2);
    inserirIterativo(&t, 4);
    inserirIterativo(&t, 8);
    inserirIterativo(&t, 9);
    n = -1;
    exibir(t, &n);
    //system("pause");
    inserirIterativo(&t, 10); // Rotação simples no 8
    n = -1;
    exibir(t, &n);
    //system("pause");
    inserirIterativo(&t, 11);
    inserirIterativo(&t, 12);
    inserirIterativo(&t, 13);
    inserirIterativo(&t, 14);
    inserirIterativo(&t, 7);
    inserirIterativo(&t, 6);
    // inserirAVL(&t, 5, &flag);
    // inserirAVL(&t, 3, &flag);
    // inserirAVL(&t, 2, &flag);
    // inserirAVL(&t, 4, &flag);
    // inserirAVL(&t, 8, &flag);
    // inserirAVL(&t, 9, &flag);
    // inserirAVL(&t, 10, &flag);
    // inserirAVL(&t, 11, &flag);
    // inserirAVL(&t, 12, &flag);
    // inserirAVL(&t, 13, &flag);
    // inserirAVL(&t, 14, &flag);


    n = -1;
    exibir(t, &n);

    // printf("Percorrer: \n");
    // Order1(t);
    // Order2(t);
    // Order3(t);

    // n = -1;
    // exibir(t, &n);

    //system("pause");

    return 0;
}