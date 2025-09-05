#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct reg_lista {
    struct gen *cabeca;
    struct gen *cauda;
};

union info_lista {
    char info[8];
    struct reg_lista lista;
};

struct gen {
    int terminal;
    union info_lista no;
};

typedef struct gen Gen;

struct fila {
    char elem[8];
    int prioridade;
    struct fila *prox;
};

typedef struct fila Fila;

#include "pilha.h"

char nula(Gen *l) {
    return l == NULL;
}

char atomo(Gen *l) {
    return !nula(l) && l->terminal;
}

Gen* Head(Gen *l) {
    if (!atomo(l) && !nula(l)) {
        return l->no.lista.cabeca;
    }
}

Gen* Tail(Gen *l) {
    if (!atomo(l) && !nula(l)) {
        return l->no.lista.cauda;
    }
}

Gen* criaT(char *info) {
    Gen* l = (Gen*)malloc(sizeof(Gen));
    l->terminal = 1;
    strcpy(l->no.info, info);
    return l;
}

Gen* cons(Gen* H, Gen* T) {
    if (!atomo(T)) {
        Gen *l = (Gen*)malloc(sizeof(Gen));
        l->terminal = 0;
        l->no.lista.cabeca = H;
        l->no.lista.cauda = T;
        return l;
    }
}

void exibir(Gen* l) {
    Pilha P;
    inicializaPilha(&P);

    printf("[");
    while (l != NULL || !pilhaVazia(&P)) {
        if (l == NULL) {
            printf("]");
            pop(&P, &l);
            l = Tail(l);
            if (!nula(l)) { printf(", "); }
        } else {
            if (!atomo(l)) {
                push(&P, l);
                l = Head(l);
                if (!atomo(l)) { printf("["); }
            } else {
                printf("%s", l->no.info);
                pop(&P, &l);
                l = Tail(l);
                if (!nula(l)) { printf(", "); }
            }
        }
    }
    printf("]");
    puts("");
}

void exibirLista(Gen* L) {

    if(atomo(L)) {
        printf("%s", L->no.info);
    }
    else {
        printf("[");
        while (!nula(L)) {
            exibirLista(Head(L));
            L = Tail(L);
            if(!nula(L)) {
                printf(", ");
            }
        }
        printf("]");
    }
}

void percorrerGen(Gen* l) {
    Pilha P;
    inicializaPilha(&P);

    while (l != NULL || !pilhaVazia(&P)) {
        if (l == NULL) { //Quando um tail der NULL
            pop(&P, &l);
            l = Tail(l);
        } else {
            if (!atomo(l)) { // Caixa comum
                push(&P, l);
                l = Head(l);
            } else { // Quando chegar em um atomo
                pop(&P, &l);
                l = Tail(l);
            }
        }
    }
}

//1:-)  Faça  um  algoritmo  que  receba  uma  Lista  Generalizada  L  por  parâmetro  e  a  retorne 
//ordenando  todos  os  átomos  de  cada  uma  de  suas  sublistas.  O  algoritmo  deve  ordenar  os 
//átomos em cada Lista Generalizada (sublista). 
//Estrategia:
//Criar função para ordenar a partir de uma cabeça de lista
//Ordenar apenas aquele nivel
//Criar função que recupera as cabeças de lista relevantes para serem utilizadas
//  na função anterior
void ordena(Gen *l) {
    Gen *ant, *atual, *at1, *at2;
    int cont = 0;
    char temp[8];

    ant = l;
    while(ant != NULL) {
        if (atomo(Head(ant))) {cont++;}
        ant = Tail(ant);
    }
    //printf("lista tem %d elementos\n", cont); system("pause");
    
    //Achar ant
    ant = l;
    while (!nula(ant) && !atomo(Head(ant))) {
        ant = Tail(ant);
    }
    for (int i = 0; i < cont; i++) { //Itera o algoritmo a quantidade de itens vezes
        while(ant != NULL) { //Itera a lista até o fim
            //Achar o atual
            atual = Tail(ant);
            while (!nula(atual) && !atomo(Head(atual))) {
                atual = Tail(atual);
            }

            //Verificar se precisa de troca
            if(!nula(atual)) {
                at1 = Head(ant);
                at2 = Head(atual);
        
                if (strcmp(at1->no.info, at2->no.info) > 0) {
                    strcpy(temp, at1->no.info);
                    strcpy(at1->no.info, at2->no.info);
                    strcpy(at2->no.info, temp);
                }
            }
            
            ant = atual;
        }
    }
    
}

void reordena(Gen *l) {
    Pilha P, P2;
    inicializaPilha(&P);
    inicializaPilha(&P2);

    push(&P2, l);
    while(l != NULL || !pilhaVazia(&P)) {
        if (l == NULL) {
            pop(&P, &l);
            l = Tail(l);
        } else {
            if (!atomo(l) ) {
                push(&P, l);
                l = Head(l);
                if (!atomo(l)) { push(&P2, l); }
            } else {
                pop(&P, &l);
                l = Tail(l);
            }
        }
    }

    while (!pilhaVazia(&P2)) {
        pop(&P2, &l);
        ordena(l);
    }
}

Gen *construirLista(char *str) { //Ex: "[x, a, [c, d], [e, a, [f]], b]" "[]"
    Gen *l, *ret = NULL, *aux;
    Pilha P;
    inicializaPilha(&P);
    char info[50];
    int i = 1, j, TL = strlen(str);

    if (TL > 2) { // "[]" seria l = NULL
        l = (Gen*)malloc(sizeof(Gen));
        l->terminal = 0;
        l->no.lista.cabeca = NULL;
        l->no.lista.cauda = NULL;
        ret = l;

        while(i < TL) {
            switch (str[i]) {
                case '[':
                    aux = (Gen*)malloc(sizeof(Gen));
                    aux->terminal = 0;
                    aux->no.lista.cabeca = NULL;
                    aux->no.lista.cauda = NULL;
                    
                    push(&P, l);
                    
                    l->no.lista.cabeca = aux;
                    l = Head(l);
                break;
                
                case ',':
                    aux = (Gen*)malloc(sizeof(Gen));
                    aux->terminal = 0;
                    aux->no.lista.cabeca = NULL;
                    aux->no.lista.cauda = NULL;
                    
                    l->no.lista.cauda = aux;
                    l = Tail(l);
                break;

                case ']':
                    pop(&P, &l);
                break;

                case ' ':
                break;

                default:
                    j = 0;
                    while (str[i] != '[' && str[i] != ',' && str[i] != ']') {
                        info[j++] = str[i++];
                    }
                    info[j] = '\0';
                    i--;

                    aux = (Gen*)malloc(sizeof(Gen));
                    aux->terminal = 1;
                    strcpy(aux->no.info, info);

                    l->no.lista.cabeca = aux;
                break;
            }
            
            i++;
        }

    }

    return ret;
}

// 2:-)  Faça  um  algoritmo  para  inserir  um  elemento  (átomo)  em  cada  uma  das  sublistas  de 
// uma  Lista  Generalizadas  L.  A  Lista  Generalizada  possui  os  átomos  em  ordem  e  o 
// elemento deve ser inserido obedecendo essa ordem. Exemplos de chamada do algoritmo:  insere(&L, “b”, 1); 
//Não insere, pois já tem o “b”!   insere(&L, “b”, 2);
void insereAtomo(char info[8], Gen** l) {
    Gen *aux, *atual, *no, *caixa;
    int flag = 0;

    caixa = (Gen*)malloc(sizeof(Gen));
    caixa->terminal = 0;
    caixa->no.lista.cabeca = NULL;
    caixa->no.lista.cauda = NULL;

    no = (Gen*)malloc(sizeof(Gen));
    no->terminal = 1;
    strcpy(no->no.info, info);

    //Trata inserir no primeiro espaço
    if (atomo(Head(*l))) {
        atual = (*l)->no.lista.cabeca;
        if (strcmp(atual->no.info, no->no.info) > 0) {
            caixa->no.lista.cabeca = atual;
            caixa->no.lista.cauda = Tail((*l));

            (*l)->no.lista.cabeca = no;
            (*l)->no.lista.cauda = caixa;
            flag = 1;
        }
    }

    if (flag == 0) {
        aux = Tail(*l);
        //Busca
        while(aux != NULL && flag == 0) {
            if (atomo(Head(aux))) {
                atual = Head(aux);
            }

            if (strcmp(atual->no.info, no->no.info) > 0) {
                caixa->no.lista.cabeca = atual;
                caixa->no.lista.cauda = Tail(aux);

                aux->no.lista.cabeca = no;
                aux->no.lista.cauda = caixa;
                flag = 1;
            }

            aux = Tail(aux);
        }  
    }

}

void insere(Gen** l, char info[8], int nivelAlvo) {
    //Verificar se ja existe
    //Se o nivel for maior que 1 precisa 
    //  recuperar todas as cabeças de sublista
    //Inserir no local correto
    Gen *aux = *l;
    Pilha P, P2;
    inicializaPilha(&P);
    inicializaPilha(&P2);
    int nivel = 1;

    if (nivel == nivelAlvo) {
        insereAtomo(info, l);
    } else {
        while(aux != NULL || !pilhaVazia(&P)) {
            if (aux == NULL) {
                pop(&P, &aux);
                aux = Tail(aux);
                nivel--;
            } else {
                if (!atomo(aux)) {
                    push(&P, aux);
                    aux = Head(aux);
                    if (!atomo(aux)) {
                        if (nivel == nivelAlvo - 1) { //Captura cabeças de lista
                            push(&P2, aux);
                        }
                        nivel++;
                    }
                } else {
                    //printf("Atomo: %s, nivel %d\n", aux->no.info, nivel);
                    pop(&P, &aux);
                    aux = Tail(aux);
                }
            }
        }

        while (!pilhaVazia(&P2)) {
            pop(&P2, &aux);
            insereAtomo(info, &aux);
        }
    }
}

// 3) Faça um algoritmo para excluir os átomos de um dado nível de Lista. Observe que os 
//          nodos de Lista que apontam para os átomos também são excluídos! 
void excluiNivel(Gen *l, int nivelAlvo) {
    Gen *aux = l, *aux2, *exc;
    Pilha p1, p2, p3;
    inicializaPilha(&p1);
    inicializaPilha(&p2);
    inicializaPilha(&p3);
    int nivel = 1;


    while(aux != NULL || !pilhaVazia(&p1)) {
        if (aux == NULL) {
            pop(&p1, &aux);
            aux = Tail(aux);
            nivel--;
        } else {
            if (!atomo(aux)) {
                push(&p1, aux);
                if (!atomo(Head(aux)) && nivel == nivelAlvo - 1) {
                    push(&p2, aux);
                }
                aux = Head(aux);
                if (!atomo(aux)) {
                    nivel++;
                }
            } else {
                pop(&p1, &aux);
                aux = Tail(aux);
            }
        }
    }

    inicializaPilha(&p1);
    while(!pilhaVazia(&p2)) {
        pop(&p2, &aux2);
        aux = Head(aux2);

        while (aux != NULL) {
            if (atomo(Head(aux))) {
                exc = aux;
                aux = Tail(aux);
                free(Head(exc));
                free(exc);
            } else {
                push(&p3, aux);
                aux = Tail(aux);
            }
        }

        while(!pilhaVazia(&p3)) {
            pop(&p3, &aux);
            push(&p1, aux);
        }

        pop(&p1, &aux);
        aux2->no.lista.cabeca = aux;
        aux2 = Head(aux2);
        while(!pilhaVazia(&p1)) {
            pop(&p1, &aux);
            aux2->no.lista.cauda = aux;
            aux2 = Tail(aux2);
        }
    }
}

Gen* limpaFileira(Gen* l) {
    Gen *cabeca = l, *aux, *ant;

    while (cabeca != NULL && Head(cabeca) == NULL) {
        aux = cabeca;
        cabeca = Tail(cabeca);
        free(aux);
    }

    ant = cabeca;
    if (cabeca != NULL) {
        aux = Tail(cabeca);

        while (aux != NULL) {
            if (Head(aux) == NULL) {
                ant->no.lista.cauda = Tail(aux);
                free(aux);
                aux = Tail(ant);
            } else {
                ant = aux;
                aux = Tail(aux);
            }
        }
    }

    return cabeca;
}


//4) Faça um algoritmo que exclua todos os nodos de Lista que apontam para Lista Nula.
void excluirNulo(Gen **l) {
    Gen *aux = *l, *aux2;
    Pilha p, p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    if (*l != NULL) {
        while(aux != NULL || !pilhaVazia(&p)) {
            if (aux == NULL) {
                pop(&p, &aux);
                aux = Tail(aux);
            } else {
                if (!atomo(aux)) {
                    push(&p, aux);
                    aux2 = aux;
                    aux = Head(aux);
                    if (!atomo(aux) && !nula(aux)) {
                        push(&p2, aux2);
                    }
                } else {
                    pop(&p, &aux);
                    aux = Tail(aux);
                }
            }
        }

        *l = limpaFileira(*l);
        
        while (!pilhaVazia(&p2)) {
            pop(&p2, &aux);
            aux->no.lista.cabeca = limpaFileira(aux->no.lista.cabeca);
        }
    }

    //LimpaFileira -> retorna ponteiro pra conectar na cabeça de lista
    //Guarda cabeças de sublista
    //Liga cabeça a Fileira
}

//5:-)  Faça  um  algoritmo  que  transforme  uma  Lista  Generalizada em  uma Fila  com 
// prioridade, sendo que cada nível dos nodos de Lista corresponde a um nível de prioridade. 
Fila *criaNo(char elem[8], int prioridade) {
    Fila *novo = (Fila *)malloc(sizeof(Fila));
    strcpy(novo->elem, elem);
    novo->prioridade = prioridade;
    novo->prox = NULL;
    return novo;
}

void insereFila(Fila **inicio, char elem[8], int prioridade) {
    Fila *novo = criaNo(elem, prioridade);
    Fila *atual = *inicio;

    if (*inicio == NULL || prioridade < (*inicio)->prioridade) {
        novo->prox = *inicio;
        *inicio = novo;
    } else {
        while (atual->prox != NULL && atual->prox->prioridade <= prioridade) {
            atual = atual->prox;
        }
        novo->prox = atual->prox;
        atual->prox = novo;
    }
}

void imprimeFila(Fila *inicio) {
    Fila *p = inicio;
    while (p != NULL) {
        printf("(%s, %d) ", p->elem, p->prioridade);
        p = p->prox;
    }
    printf("\n");
}

void criaFila(Gen *lista, Fila **f) {
    int nivel = 1;
    Pilha p;
    inicializaPilha(&p);

    while(lista != NULL || !pilhaVazia(&p)) {
        if (lista == NULL) {
            pop(&p, &lista);
            lista = Tail(lista);
            nivel--;
        } else {
            if (!atomo(lista)) {    
                push(&p, lista);
                if (atomo(Head(lista))) {
                    insereFila(f, lista->no.lista.cabeca->no.info, nivel);
                }
                lista = Head(lista);
                if (!atomo(lista)) {
                    nivel++;
                }
            } else {
                pop(&p, &lista);
                lista = Tail(lista);
            }
        }
    }
}

//6:-)  Faça  um  algoritmo  que  coloque  todos  os  átomos  de  uma  Lista  
// Generalizada  L  no bottom-level.
void botton(Gen* lista) {
    int nivel = 1, maior = 0;
    Gen *l = lista, *aux;
    Pilha p;
    inicializaPilha(&p);

    while(!nula(l) || !pilhaVazia(&p)) {
        if (nula(l)) {
            pop(&p, &l);
            l = Tail(l);
            nivel--;
        } else {
            if (!atomo(l)) {
                push(&p, l);
                l = Head(l);
                if (!atomo(l)) {
                    nivel++;
                    maior = (nivel > maior) ? nivel : maior;
                }
            } else {
                pop(&p, &l);
                l = Tail(l);
            }
        }
    }

    nivel = 1;
    l = lista;
    while(!nula(l) || !pilhaVazia(&p)) {
        if (nula(l)) {
            pop(&p, &l);
            l = Tail(l);
            nivel--;
        } else {
            if (!atomo(l)) {
                push(&p, l);

                if (atomo(Head(l)) && nivel < maior) {
                    aux = l;
                    for (int i = nivel; i < maior; i++) {
                        aux->no.lista.cabeca = cons(aux->no.lista.cabeca, NULL);
                        aux = Head(aux);
                    }
                }

                l = Head(l);
                if (!atomo(l)) { nivel++; }
            } else {
                pop(&p, &l);
                l = Tail(l);
            }
        }
    }
}

// 7:-)  Faça  um  algoritmo  utilizando  Listas  Generalizadas  para  manipular  categorias  de 
// filmes, sendo que cada categoria pode ser composta de nenhuma, uma ou mais 
// subcategorias. As funções a serem implementadas são:  
 
// inserirCategoria(ListaGen* pai, ListaGen* filho) 
// Insere a categoria filho como filha da categoria pai. 
void inserirCategoria(Gen* pai, Gen* filho) {
    Gen *aux = Head(pai);
    Gen *ant;

    if (aux == NULL) {
        pai->no.lista.cabeca = filho;
    } else {
        ant = aux;
        aux = Tail(aux);
        while (aux != NULL && stricmp(ant->no.info, aux->no.info) < 0) {
            ant = aux;
            aux = Tail(aux);
        }

        if (aux == NULL) {
            ant->no.lista.cauda = filho;
        } else {
            ant->no.lista.cauda = filho;
            filho->no.lista.cauda = aux;
        }
    }
}
 
// retirarCategoria(ListaGen* pai, char* nome); 
// Retira uma categoria procurando seu nome através da busca a partir de seu pai. 
 
// exibirCategorias(ListaGen* pai) 
// Exibe a lista de categorias e subcategorias da seguinte maneira (deverá incluir a 
// tabulação): 
// Comédia
//     Policial
//     Romântica
// Terror
//     Zumbi
//     Vampiro
// Ação
//     Aventura
//         Anime
//         Militar
// Ficção Científica
//     Poderes
//     Invasão espacial 
 
// procurarCategoria(ListaGen* pai, char* nome) 
// Retorna a Lista Generalizada filha que se encaixa no parâmetro nome passado.  
// Exemplo da estrutura: 
// L  =  [“Comédia”,[“Policial”,”Romântica”]],[“Terror”,[“Zumbi”,Vampiro”]],[“Ação”,[“Aventura”, [“Anime”, 
// “Militar”]]], [“Ficção Científica”,[“Poderes”, “Invasão espacial”]] 



//===============================================================================================
//========================================== Lista 2 ============================================
//===============================================================================================

// 1:-) Fazer um algoritmo para destruir uma lista generalizada (liberar todos os nodos de uma lista). 
// Três casos a considerar: 
// • L tem valor nulo, representando uma lista vazia; 
// • L aponta um nodo terminal, representando um átomo; 
// • L aponta um nodo não terminal, representando uma lista generalizada.

void destruir(Gen *l) {
    Pilha p, p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    if (l != NULL) {
        while(l != NULL || !pilhaVazia(&p)) {
            if (l == NULL){
                pop(&p, &l);
                l = Tail(l);
            } else {
                push(&p, l);
                push(&p2, l);
                l = Head(l);
            }
        }

        while (!pilhaVazia(&p2)) {
            pop(&p2, &l);
            free(l);
        }
    }
}

//2:-) Fazer um algoritmo para duplicar uma lista generalizada fornecida por parâmetro.
Gen * duplicar(Gen *l) {
    Gen *ret, *aux;
    Pilha p;
    Pilha p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    aux = cons(NULL, NULL);  // raiz da cópia
    ret = aux;

    if (l != NULL) {
        push(&p, l);
        while(!pilhaVazia(&p)) {
            pop(&p, &l);

            if (l != NULL) {
                if (Head(l) != NULL) {
                    if (atomo(Head(l))) {
                        aux->no.lista.cabeca = criaT(l->no.lista.cabeca->no.info);
                    } else {
                        aux->no.lista.cabeca = cons(NULL, NULL);
                    }
                }
                push(&p, Head(l));
                if (Tail(l) != NULL) {
                    aux->no.lista.cauda = cons(NULL, NULL);
                }
                push(&p, Tail(l));
            }
        }
    }

    return ret;
}

// 3:-)  Fazer  um  algoritmo  (função  boolean)  para  comparar  duas  listas  generalizadas  passadas  por 
// parâmetro.
char compara(Gen *l, Gen *l2) {
    char flag = 1;
    Pilha p, p2, p3, p4;
    inicializaPilha(&p);
    inicializaPilha(&p2);
    inicializaPilha(&p3);
    inicializaPilha(&p4);

    if (l != NULL) {
        while(l != NULL || !pilhaVazia(&p)) {
            if (l == NULL){
                pop(&p, &l);
                l = Tail(l);
            } else {
                push(&p, l);
                l = Head(l);
                if (atomo(l)) { push(&p2, l); }
            }
        }
    }

    if (l2 != NULL) {
        while(l2 != NULL || !pilhaVazia(&p4)) {
            if (l2 == NULL){
                pop(&p4, &l2);
                l2 = Tail(l2);
            } else {
                push(&p4, l2);
                l2 = Head(l2);
                if (atomo(l2)) { push(&p3, l2); }
            }
        }
    }


    while (!pilhaVazia(&p2) && !pilhaVazia(&p3)) {
        pop(&p2, &l);
        pop(&p3, &l2);

        if (strcmp(l->no.info, l2->no.info) != 0) {
            flag = 0;
        }
    }
    
    if (!pilhaVazia(&p2) || !pilhaVazia(&p3)) {
        flag = 0;
    }

    return flag;
}


int main(void) {
    //Gen *L = NULL;
    // L = (Gen*)malloc(sizeof(Gen));
    // L = cons(criaT("aaa"), cons(criaT("bbb"), cons(cons(criaT("ccc"), NULL), NULL)));
    // exibir(L);
    // puts("");
    
    char* entrada = "[x, a, [c, d], [], [e, a, [f]], b]";
    //char* teste = "[]"; printf("%d", strlen(teste)); 
    Gen* lista = construirLista(entrada);
    Gen* lista2 = construirLista(entrada);

    printf("Listas %s", compara(lista2, lista) ? "iguais" : "diferentes");

    exibir(lista); //exibirLista(lista); puts("");
    reordena(lista);
    exibir(lista);
    
    insere(&lista, "b", 1); //Não insere, pois já tem o “b”!
    insere(&lista, "b", 2);
    exibir(lista);

    excluiNivel(lista, 2);
    exibir(lista);

    excluirNulo(&lista); // Precisa chamar mais uma vez
    excluirNulo(&lista);
    insere(&lista, "c", 3);
    exibir(lista);
    
    Fila *f = NULL;
    criaFila(lista, &f);
    imprimeFila(f);
    
    botton(lista);
    exibir(lista);
    
    // Gen* dup = duplicar(lista);
    // exibir(lista);
    // exibir(dup);

    // char *cat = "[“Comédia”,[“Policial”,”Romântica”]],"
    //             "[“Terror”,[“Zumbi”,Vampiro”]],"
    //             "[“Ação”,[“Aventura”, [“Anime”,“Militar”]]],"
    //             "[“Ficção Científica”,[“Poderes”, “Invasão espacial”]]";

    // Gen *pai = (Gen*)malloc(sizeof(Gen));
    // pai->terminal = 0;
    // pai->no.lista.cabeca = NULL;
    // pai->no.lista.cauda = NULL;

    // Gen *filho = (Gen*)malloc(sizeof(Gen));

    system("pause");
    return 0;
}

