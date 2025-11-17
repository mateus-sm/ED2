#include <stdio.h>

struct tree {
    char info[8];
    struct tree *esq;
    struct tree *dir;
};
typedef struct tree Tree;

#include "pilha.h"
#include "fila.h"


//Criar um no
Tree* criaNo(char* str) {
    Tree *no = (Tree*)malloc(sizeof(Tree));
    strcpy(no->info, str);
    no->esq = NULL;
    no->dir = NULL;
    return no;
}

//Inicializar uma arvore binaria de busca
void inicializaABB(Tree** raiz) {
    *raiz = criaNo("*");
}

//Inserir na arvore ABB
void inserir(Tree **raiz, char* str) {
    Tree *no, *ant;
    if (*raiz == NULL) {
        *raiz = criaNo(str);
    } else {
        no = *raiz;
        ant = no;
        
        while (no != NULL) {
            ant = no;

            if (strcmp(str, no->info) > 0) {
                no = no->dir;
            } else {
                no = no->esq;
            }
        }

        if (strcmp(str, ant->info) > 0) {
            ant->dir = criaNo(str);
        } else {
            ant->esq = criaNo(str);
        }
    }
}

//Exibir ABB
void exibirABB(Tree *arv, int *n) {
    if (arv != NULL) {
        if((*n) == -1) {
            printf("Arvore: \n");
        }
        (*n)++;
        exibirABB(arv->dir, n);
        for(int i = 0; i < 5 * (*n); i++) { printf(" "); }
        printf("(%s)\n", arv->info);
        exibirABB(arv->esq, n);
        (*n)--;
    }
}

//Busca
Tree* busca(Tree* raiz, char *str) {
    Tree* no = raiz, *achou = NULL;
    Pilha p;
    inicializaPilha(&p);

    while (no != NULL || !pilhaVazia(&p)) {
        if (no == NULL) {
            pop(&p, &no);
            no = no->dir;
        } else {
            if (strcmp(no->info, str) == 0) {
                achou = no;
            }
            push(&p, no);
            no = no->esq;
        }
    }

    return achou;
}

//Faça um algoritmo que retorne por referência ou por return o nível (ou profundidade)
// de um dado nó de uma Árvore Binária de Busca.
int nivel(Tree* arv) {
    int nivel = 1;
    Tree* no = arv;
    Pilha p;
    inicializaPilha(&p);

    while (no != NULL || !pilhaVazia(&p)) {
        if (no == NULL) {
            pop(&p, &no);
            nivel--;
            no = no->dir;
            if (no != NULL) { nivel++; }
        } else {
            printf("No: %s nivel: %d\n", no->info, nivel);
            push(&p, no);
            no = no->esq;
            nivel++;
        }
    }
}

//Faça um algoritmo que retorne o pai de um dado nó de uma Árvore Binária de Busca.
Tree* buscaPai(Tree *raiz, char *info) {
    Tree *no, *ant;
    no = raiz;
    ant = no;

    while (no != NULL && strcmp(info, no->info) != 0) {
        ant = no;

        if (strcmp(info, no->info) > 0) {
            no = no->dir;
        } else {
            no = no->esq;
        }
    }

    return ant;
}

// Faça algoritmos de percurso em Árvores Binárias (algoritmos de travessia) de forma
// iterativa.
// a) Pré-ordem
// b) In-ordem
// c) Pós-ordem
void preOrdem(Tree *arv) {
    Pilha p;
    inicializaPilha(&p);

    printf("Pre ordem: \n");
    while (arv != NULL || !pilhaVazia(&p)) {
        if (arv == NULL) {
            pop(&p, &arv);
            arv = arv->dir;
        } else {
            printf("%s ", arv->info);
            push(&p, arv);
            arv = arv->esq;
        }
    }

    puts("");
}

void inOrdem(Tree *arv) {
    Pilha p;
    inicializaPilha(&p);

    printf("In ordem: \n");
    while (arv != NULL || !pilhaVazia(&p)) {
        if (arv == NULL) {
            pop(&p, &arv);
            printf("%s ", arv->info);
            arv = arv->dir;
        } else {
            push(&p, arv);
            arv = arv->esq;
        }
    }

    puts("");
}

//Estrategia: Percorrer por nivel, guarde o no atual e desça primeiro esq depois dir(em uma pilha)
void posOrdem(Tree *arv) {
    Pilha p, p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    push(&p, arv);
    while (!pilhaVazia(&p)) {
        pop(&p, &arv);

        if (arv != NULL) {
            push(&p2, arv);
            push(&p, arv->esq);
            push(&p, arv->dir);
        }
    }

    printf("Pos ordem: \n");
    while (!pilhaVazia(&p2)) {
        pop(&p2, &arv);
        printf("%s ", arv->info);
    }

    puts("");
}

// Faça um algoritmo para torar uma árvore Vazia (free em todos os nodos da árvore)
// Estrategia: percorrer normal e apagar junto
void apagar(Tree **arv) {
    Tree *exc;
    Pilha p, p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    while (*arv != NULL || !pilhaVazia(&p)) {
        if (*arv == NULL) {
            pop(&p, arv);
            exc = *arv;
            *arv = (*arv)->dir;
            free(exc);
        } else {
            push(&p, *arv);
            push(&p2, *arv);
            *arv = (*arv)->esq;
        }
    }
}

int main (void) {
    Tree* abb;
    inicializaABB(&abb);

    char *nomes[] = {
        "Bia", "Ana", "Caio", "Mat", "Diana",
        "Eli", "Zoe", "Lara", "Gabi", "Nina",
    };

    int qtd = sizeof(nomes) / sizeof(nomes[0]);

    for (int i = 0; i < qtd; i++) {
        inserir(&abb, nomes[i]);
    }

    int n = -1;
    exibirABB(abb, &n);

    Tree* no;
    no = busca(abb, "Mat");
    printf("\nNo buscado: %s\n", no->info);
    puts("");

    nivel(abb);

    char nome[8];
    strcpy(nome, "Ana");
    no = buscaPai(abb, nome);
    printf("\nPai de '%s': %s\n\n", nome, no->info);

    preOrdem(abb);
    inOrdem(abb);
    posOrdem(abb);

    apagar(&abb);
    exibirABB(abb, &n);

    system("pause");
    return 0;
}