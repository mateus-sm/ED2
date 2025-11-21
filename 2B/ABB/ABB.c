#include <stdio.h>
#include <stdlib.h>

struct tree {
    int info;
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

void listaDupla(Tree **t) {
    Tree *raiz = *t, *lista = NULL, *ant = NULL;
    Pilha P1, P2;
    inicializaPilha(&P1);
    inicializaPilha(&P2);

    push(&P1, raiz);
    while(!pilhaVazia(&P1)) {
        pop(&P1, &raiz);
        
        if (raiz != NULL) {
            push(&P2, raiz);
            push(&P1, raiz->esq);
            push(&P1, raiz->dir);
        }
    }
    
    while(!pilhaVazia(&P2)) {
        pop(&P2, &raiz); 
        raiz->esq = raiz->dir = NULL;

        if (lista == NULL) {
            lista = raiz;
            ant = lista;
        } else {
            ant->dir = raiz;
            raiz->esq = ant;
            ant = raiz;
        }
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

void exclusao(Tree **raiz, Tree *e, Tree *pai, char lado) {
    Tree *paisub, *sub;
    int aux;

    if (e->esq == NULL && e->dir == NULL) { //Folha?
        if (e != pai) { //Arvore tem mais que um nó?
            if (e->info > pai->info) {
                pai->dir = NULL;
            } else {
                pai->esq = NULL;
            }
        } else {
            *raiz = NULL;
        }
        free(e);
    } else { //Tem um ou mais filhos
        if (e->esq == NULL || e->dir == NULL) { // Tem um filho
            if(e != pai) {
                if (e->info < pai->info) {
                    if (e->esq != NULL) {
                        pai->esq = e->esq;
                    } else {
                        pai->esq = e->dir;
                    }
                } else {
                    if(e->esq != NULL) {
                        pai->dir = e->esq;
                    } else {
                        pai->dir = e->dir;
                    }
                }
            } else {
                if (e->esq != NULL) {
                    *raiz = e->esq;
                } else {
                    *raiz = e->dir;
                }
            }
            free(e);
        } else { // Tem dois filhos
            if (lado == 'd') {
                paisub = e;
                sub = e->dir;
                while (sub->esq != NULL) {
                    paisub = sub;
                    sub = sub->esq;
                }
                aux = sub->info;
                exclusao(&(*raiz), sub, paisub, lado);
                e->info = aux;                
            } else {
                paisub = e;
                sub = e->esq;
                while (sub->dir != NULL) {
                    paisub = sub;
                    sub = sub->dir;
                }
                aux = sub->info;
                exclusao(&(*raiz), sub, paisub, lado);
                e->info = aux;
            }
        }
    }
}

void busca(Tree *raiz, int info, Tree **e, Tree **pai) {
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

void balanceamento(Tree **raiz) {
    Tree *no = NULL, *e = NULL, *pai = NULL;
    Fila F;
    inicializaFila(&F); 
    int qdir, qesq, fb, aux, n;
    
    if (*raiz != NULL) {
        //percorre por nivel
        enqueue(&F, *raiz);
        while (!filaVazia(&F)) {
            dequeue(&F, &no);
            
            //balancear
            do {
                //calcula o fator de balanceamento do elemento
                qdir = qesq = 0;
                quantNo(no->dir, &qdir); //printf("pai = %d, dir = %d", no->info, qdir);
                quantNo(no->esq, &qesq); //printf(" esq = %d", qesq); system("pause");
                fb = qdir - qesq;
                
                //se encaixar no criterio balanceia
                if (fb > 1 || fb < -1) {
                    aux = no->info;
                    
                    //buca o pai do no
                    busca(*raiz, aux, &e, &pai);
                    //Se tver só um filho vai pra ele
                    no = (no->esq == NULL) ? no->dir : (no->dir == NULL) ? no->esq : no;
                    
                    //excluir o elemento
                    if (fb > 0) {exclusao(&*raiz, e, pai, 'd');} 
                    else {exclusao(&*raiz, e, pai, 'e');}
                    
                    //inserir o elemento de volta
                    insereArv(&*raiz, aux);
                }
                
            } while (fb > 1 || fb < -1);
            
            if (no->esq != NULL) {enqueue(&F, no->esq);} 
            if (no->dir != NULL) {enqueue(&F, no->dir);}
        } 
    }
}

void altura(Tree *raiz, int nivel, int *maior) {
    if (raiz != NULL) {
        if (nivel > *maior) {
            *maior = nivel;
        }
        altura(raiz->esq, nivel + 1, &*maior);
        altura(raiz->dir, nivel + 1, &*maior);
    }
}

int main() {
    Tree *t = NULL, *pai = NULL, *e = NULL;
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
    insereArvRecusrsivo(&t, 10);
    insereArvRecusrsivo(&t, 11);
    insereArvRecusrsivo(&t, 12);
    insereArvRecusrsivo(&t, 13);
    insereArvRecusrsivo(&t, 14);
    
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
    //listaDupla(&t);
    // while (t->dir != NULL) {
    //     printf("%d ", t->info);
    //     t = t->dir;
    // }
    // printf("|| ");
    // while (t != NULL) {
    //     printf("%d ", t->info);
    //     t = t->esq;
    // }
    // system("pause");

    printf("\nAntecessores: \n");
    antecessores(t, 2);
    antecessores(t, 4);
    antecessores(t, 9);

    busca(t, 3, &e, &pai); //printf("e = %d, pai = %d\n", e->info, pai->info); //system("pause");
    exclusao(&t, e, pai, 'd');
    n = -1;
    exibirABB(t, &n);
    
    balanceamento(&t);
    n = -1;
    exibirABB(t, &n);

    // pai = Pai(t, 40);
    // printf("%d\n", pai->info);
    //system("pause");

    return 0;
}