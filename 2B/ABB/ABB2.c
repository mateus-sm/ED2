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
Tree* buscaRet(Tree* raiz, char *str) {
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
void nivel(Tree* arv) {
    int nivel = 1, aux = 0;
    Tree* no = arv;
    Pilha p;
    inicializaPilha(&p);

    while (no != NULL || !pilhaVazia(&p)) {
        if (no == NULL) {
            pop(&p, &no);
            nivel--;
            no = no->dir;
            nivel++;

            if (no != NULL) {
                aux++;
            } else {
                nivel -= aux;
                aux = 0;
            }
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

void exclusao(Tree **arv, Tree *e, Tree *pai, char lado) {
    Tree *paisub, *sub;
    char nome[8];

    //1° Caso, Excluir elemento de uma folha
    if (e->esq == NULL && e->dir == NULL) {
        //Verificar se exc e raiz são iguais
        if (e != *arv) {
            //Achar o filho pela info
            if (strcmp(e->info, pai->info) > 0) {
                pai->dir = NULL;
            } else {
                pai->esq = NULL;
            }
        } else {
            *arv = NULL;
        }
        free(e);
    }
    else {
        //2° Caso, Excluir elemento com 1 filho
        if (e->esq == NULL || e->dir == NULL) {
            //Verificar se exc e raiz são iguais
            if (e != *arv) {
                //Verificar o lado que tá o filho
                if (strcmp(e->info, pai->info) > 0) {
                    if (e->esq != NULL) {
                        pai->dir = e->esq;
                    } else {
                        pai->dir = e->dir;
                    }
                } else {
                    if (e->esq != NULL) {
                        pai->esq = e->esq;
                    } else {
                        pai->esq = e->dir;
                    }
                }
            } else {
                if (e->esq != NULL) {
                    *arv = e->esq;
                } else {
                    *arv = e->dir;
                }
            }
            free(e);
        }
        else {
            //3° Caso, Excluir elemento com 2 filhos
            paisub = e;
            //Busca um substituto e coloca no lugar do excluido
            if (lado == 'd') {
                sub = e->dir;
                while (sub->esq != NULL) {
                    paisub = sub;
                    sub = sub->esq;
                }
            } else {
                sub = e->esq;
                while (sub->dir != NULL) {
                    paisub = sub;
                    sub = sub->dir;
                }
            }
            strcpy(nome, sub->info);
            exclusao(&(*arv), sub, paisub, lado);
            strcpy(e->info, nome);
        }
    }
}

void busca(Tree *raiz, char nome[8], Tree **e, Tree **pai) {
    *e = *pai = raiz;

    while (*e != NULL && strcmp((*e)->info, nome) != 0) {
        *pai = *e;
        if (strcmp((*e)->info, nome) > 0) {
            *e = (*e)->esq;
        } else {
            *e = (*e)->dir;
        }
    }
}

int quantNo(Tree *no) {
    Pilha p;
    inicializaPilha(&p);
    int i = 0;

    while (no != NULL || !pilhaVazia(&p)) {
        if (no == NULL) {
            pop(&p, &no);
            i++;
            no = no->dir;
        } else {
            push(&p, no);
            no = no->esq;
        }
    }

    return i;
}

void balanceamento(Tree **arv) {
    int fb, nodir, noesq;
    char aux[8];
    Tree *no = NULL, *e = NULL, *pai = NULL;

    Fila f;
    inicializaFila(&f);

    enqueue(&f, *arv);
    while (!filaVazia(&f)) {
        dequeue(&f, &no);

        do {
            nodir = noesq = 0;
            nodir = quantNo(no->dir);
            noesq = quantNo(no->esq);
            fb = nodir - noesq;

            if (fb > 1 || fb < -1) {
                strcpy(aux, no->info);

                busca(*arv, aux, &e, &pai);
                no = (no->esq == NULL) ? no->dir : (no->dir == NULL) ? no->esq : no;

                if (fb > 0) {
                    exclusao(&*arv, e, pai, 'd');
                } else {
                    exclusao(&*arv, e, pai, 'e');
                }

                inserir(&(*arv), aux);
            }

        } while (fb > 1 || fb < -1);

        if (no->esq != NULL) { enqueue(&f, no->esq); }
        if (no->dir != NULL) { enqueue(&f, no->dir); }
    }
}

int main (void) {
    Tree* abb = NULL;
    //inicializaABB(&abb);

    //Inserir
    char *nomes[] = {
        "Bia", "Ana", "Caio", "Mat", "Diana",
        "Eli", "Zoe", "Lara", "Gabi", "Nina",
    };

    int qtd = sizeof(nomes) / sizeof(nomes[0]);

    for (int i = 0; i < qtd; i++) {
        inserir(&abb, nomes[i]);
    }

    //Exibir
    int n = -1;
    exibirABB(abb, &n);

    //Buscar
    Tree* no;
    no = buscaRet(abb, "Mat");
    printf("\nNo buscado: %s\n", no->info);
    puts("");

    //Nivel
    nivel(abb);

    //Buscar pai
    char nome[8];
    strcpy(nome, "Ana");
    no = buscaPai(abb, nome);
    printf("\nPai de '%s': %s\n\n", nome, no->info);

    //Exibir em ordens diferentes
    preOrdem(abb);
    inOrdem(abb);
    posOrdem(abb);

    //Exclusao de um nó
    Tree *e, *pai;
    strcpy(nome, "Mat");
    busca(abb, nome, &e, &pai);
    exclusao(&abb, e, pai, 'e');
    exibirABB(abb, &n);

    //Balancear arvore
    balanceamento(&abb);
    exibirABB(abb, &n);

    //Apagar arvore
    apagar(&abb);
    exibirABB(abb, &n);

    //system("pause");
    return 0;
}