#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct reg_lista {
    struct gen *cabeca;
    struct gen *calda;
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
        return l->no.lista.calda;
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
        l->no.lista.calda = T;
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
        l->no.lista.calda = NULL;
        ret = l;

        while(i < TL) {
            switch (str[i]) {
                case '[':
                    aux = (Gen*)malloc(sizeof(Gen));
                    aux->terminal = 0;
                    aux->no.lista.cabeca = NULL;
                    aux->no.lista.calda = NULL;
                    
                    push(&P, l);
                    
                    l->no.lista.cabeca = aux;
                    l = Head(l);
                break;
                
                case ',':
                    aux = (Gen*)malloc(sizeof(Gen));
                    aux->terminal = 0;
                    aux->no.lista.cabeca = NULL;
                    aux->no.lista.calda = NULL;
                    
                    l->no.lista.calda = aux;
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
    caixa->no.lista.calda = NULL;

    no = (Gen*)malloc(sizeof(Gen));
    no->terminal = 1;
    strcpy(no->no.info, info);

    //Trata inserir no primeiro espaço
    if (atomo(Head(*l))) {
        atual = (*l)->no.lista.cabeca;
        if (strcmp(atual->no.info, no->no.info) > 0) {
            caixa->no.lista.cabeca = atual;
            caixa->no.lista.calda = Tail((*l));

            (*l)->no.lista.cabeca = no;
            (*l)->no.lista.calda = caixa;
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
                caixa->no.lista.calda = Tail(aux);

                aux->no.lista.cabeca = no;
                aux->no.lista.calda = caixa;
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

int main(void) {
    //Gen *L = NULL;
    // L = (Gen*)malloc(sizeof(Gen));
    // L = cons(criaT("aaa"), cons(criaT("bbb"), cons(cons(criaT("ccc"), NULL), NULL)));
    // exibir(L);
    // puts("");
    
    char* entrada = "[x, a, [c, d], [e, a, [f]], b]";
    //char* teste = "[]"; printf("%d", strlen(teste)); 
    Gen* lista = construirLista(entrada);
    exibir(lista); //exibirLista(lista); puts("");
    reordena(lista);
    exibir(lista);
    
    insere(&lista, "b", 1); //Não insere, pois já tem o “b”!
    insere(&lista, "b", 2);
    exibir(lista);

    system("pause");
    return 0;
}

