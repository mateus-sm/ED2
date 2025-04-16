#include <stdio.h>
#include <stdlib.h>

struct tree {
    int info;
    struct tree *esq, *dir;
};

typedef struct tree Tree;

//Inicializar uma árvore binária;
void inicializaArv(Tree **t) {
    *t = NULL;
}

//Criar um nó
void criaNoArv(Tree **t, int info) {
    Tree *aux;

    if (*t == NULL) {
        aux = (Tree*)malloc(sizeof(Tree));
        aux->info = info;
        aux->dir = NULL;
        aux->esq = NULL;
        *t = aux;
    } 
    
}

//Verificar se a árvore está vazia ou não
char arvVazia(Tree *t) {
    return t == NULL;
}

Tree *localizaNo(Tree *t, int no) {
    Tree *no = NULL;

    

    return no;
}

// Criar um filho à direita de um dado nó; 
// Criar um filho à esquerda de um dado nó;
void insereArv(Tree **t, int info, int pai_info, char dir_info) {
    Tree *pai = NULL;


    if (arvVazia(*t)) {
        criaNoArv(*t, info);   
    } else {
        pai = localizaNo(*t, pai_info);

        if(pai->info == pai_info) {
            if (dir_info == 'D' && pai->dir != NULL) {
                criaNoArv(&pai->dir, info);
            } 
            else {
                if (dir_info == 'E' && pai->esq != NULL) {
                    criaNoArv(&pai->esq, info);
                }
            }
        }
    }
}

int main() {
    Tree *t;

    inicializaArv(&t);
    printf("%d\n", arvVazia(t));
    
    criaNoArv(&t, 10);
    printf("%d\n", t->info);
    
    insereArv(&t, 20, 10, 'E');
    printf("%d\n", t->esq->info);

    system("pause");
    return 0;
}