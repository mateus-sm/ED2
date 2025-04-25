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

//1)  Utilizar  os  algoritmos  de  travessia  para  fazer  uma  busca  na  Árvore  (localizar  um 
// determinado elemento passado por parâmetro, retornando: achou true (1) ou false (0), e 
// se achou, retornar o nodo que se encontra o elemento). 
//Estratégia: Precisa percorrer ela inteira (Pré Ordem)
//Razão: arvore sem ordenação
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

//3) Verificar qual o nível (ou profundidade) de um dado nó.
//Arvore comum, precisa percorrer ela por inteiro
//Estratégia: Esq e Dir adicionam um ao contador.
//            Pop retira um do contador. Pois a raiz estará em NULL.
//            Se percorrer mais de uma vez para direita
//              é necessario contar os nives e remove-los
//              quando uma das direitas for nula.
//              Isso ocorre devido a maneira a qual a logica percorre
//              o codigo, os nos da direita só ficam um ciclo dentro
//              da pilha.
int nivel(Tree *t, int info) {
    Tree *raiz = t;
    Pilha P;
    int i = 1, nivel = 0, aux = 1;

    inicializaPilha(&P);

    while(raiz != NULL || !pilhaVazia(&P)) {
        if (raiz == NULL) { //Cheguei no max esquerda
            pop(&P, &raiz); //Volto para o pai
            i--;

            if (raiz->info == info) {
                nivel = i;
            }

            raiz = raiz->dir; //Vou para a direita
            i++;

            // Ao ir para a direita, se o no existir, precisa se controlar
            //  quantos niveis serão descidos, para que os mesmo sejam
            //  subtraidos no proximo pop(dir == NULL). 
            if (raiz != NULL) {
                aux++;
            } else {
                //Quando NULL vier da direita pop subtrai n(aux) direitas
                i = i - aux;
                aux = 1;
            }
        }
        else {
            if (raiz->info == info) {
                nivel = i;
            }
            push(&P, raiz);
            raiz = raiz->esq;
            i++;
        }
    }

    return nivel;
}

//2) Tornar uma árvore Vazia (free em todos os nodos da árvore). 
//Estratégia: Percorrer inteira (Pré Ordem)
//Razão: precisa-se excluir todos os nós
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

void Order1(Tree *t) {
    Tree *raiz = t;
    Pilha P;

    inicializaPilha(&P);

    while(raiz != NULL || !pilhaVazia(&P)) {
        if (raiz == NULL) { //Cheguei no max esquerda
            pop(&P, &raiz); //Volto para o pai
            //printf("%d ", raiz->info); //Pós Ordem
            raiz = raiz->dir; //Vou para a direita
        }
        else {
            //printf("%d ", raiz->info); //Pré Ordem
            push(&P, raiz);
            raiz = raiz->esq;
        }
    }

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
            printf("%d ", raiz->info); //Pós Ordem
            raiz = raiz->dir; //ir para direita
            if(raiz != NULL) {
                push(&P, raiz); //Se direita existe guarda
            }
        }
    }
}

//4) Retornar o pai de um dado nó.
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

//6308846
int main() {
    Tree *t = NULL, *pai = NULL;

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
    Order1(t);
    puts("");
    Order2(t);
    puts("");

    pai = Pai(t, 40);
    printf("%d\n", pai->info);

    system("pause");
    return 0;
}