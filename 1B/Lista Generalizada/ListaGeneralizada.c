#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct reg_lista {
    struct listagen *cabeca;
    struct listagen *calda;
};

union reg_info {
    char info[8];
    struct reg_lista no;
};

struct listagen {
    char terminal;
    union reg_info lista;
};
typedef struct listagen LG;

char nula(LG* L);
char atomo(LG* L);
LG* Head(LG* L);
LG* Tail(LG* L);
LG* criaT(char *info);
LG* cons(LG* H, LG* T);
void exibir(LG* L);
void exibirLista(LG* L);
LG* construirLista(char* frase);

#include "pilha.h"

///////////////////////////////////////////////////////
int main() {
    char* entrada = "[aaa,[bbb,ccc]]";
    LG* lista = construirLista(entrada);
    LG *L = NULL;

    L = cons(criaT("aaa"), cons(criaT("bbb"), NULL));
    exibir(L);
    exibirLista(lista);

    puts("");
    system("pause");
    return 0;
}

char nula(LG* L) {
    return L == NULL;
}

char atomo(LG *L) {
    return !nula(L) && L->terminal;
}

LG* Head(LG* L) {
    if (!nula(L) && !atomo(L)) {
        return L->lista.no.cabeca;
    }
}

LG* Tail(LG* L) {
    if(!nula(L) && !atomo(L)) {
        return L->lista.no.calda;
    }
}

LG* criaT(char *info) {
    LG* L = (LG*)malloc(sizeof(LG));
    L->terminal = 1;
    strcpy(L->lista.info, info);
    return L;
}

LG* cons(LG* H, LG* T) {
    if (atomo(T)) {
        return NULL;
    }
    else {
        LG* L = (LG*)malloc(sizeof(LG));
        L->terminal = 0;
        L->lista.no.cabeca = H;
        L->lista.no.calda = T;
        return L;
    }
}

void exibir(LG* L) {
    Pilha P;
    inicializaPilha(&P);

    push(&P, L);
    while(!pilhaVazia(&P)) {
        if(!nula(L)) {
            pop(&P, &L);

            while(!atomo(L) && !nula(L)) {
                push(&P, L);
                L = Head(L);
            }

            if(atomo(L)) {
                printf("%s\n", L->lista.info);
            }
        }

        pop(&P, &L);
        L = Tail(L);
        if (!nula(L)) {
            push(&P, L);
        }
    }
} 

void exibirLista(LG* L) {

    if(atomo(L)) {
        printf("%s", L->lista.info);
    }
    else {
        printf("[");
        while (!nula(L)) {
            exibirLista(Head(L));
            L = Tail(L);
            if(!nula(L)) {
                printf(",");
            }
        }
        printf("]");
    }
}

LG* construirLista(char* frase) {
    LG* inicio = NULL;
    if (strlen(frase) > 2) {
        Pilha P;
        LG *aux = NULL;
        int i = 1;  // Começa no segundo caractere (assumindo formato [elementos])
        
        // Cria nó inicial
        inicio = cons(NULL, NULL);
        
        aux = inicio;
        inicializaPilha(&P);
        
        while(aux != NULL && i < strlen(frase)-1) {  // Ignora último ']'
            if(frase[i] == '[') {
                push(&P, aux);
                // Cria nova lista como cabeça
                aux->lista.no.cabeca = (LG*)malloc(sizeof(LG));
                aux->lista.no.cabeca->terminal = 0;
                aux->lista.no.cabeca->lista.no.cabeca = NULL;
                aux->lista.no.cabeca->lista.no.calda = NULL;
                aux = aux->lista.no.cabeca;
            }
            else if(frase[i] == ']') {
                if(!pilhaVazia(&P)) {
                    pop(&P, &aux);
                }
                else {
                    aux = NULL;
                }
            }
            else if(frase[i] == ',') {
                // Cria nova calda
                aux->lista.no.calda = (LG*)malloc(sizeof(LG));
                aux->lista.no.calda->terminal = 0;
                aux->lista.no.calda->lista.no.cabeca = NULL;
                aux->lista.no.calda->lista.no.calda = NULL;
                aux = aux->lista.no.calda;
            }
            else {
                // Extrai a palavra
                int start = i;
                while(i < strlen(frase)-1 && (frase[i] != '[' && frase[i] != ']' && frase[i] != ',')) {
                    i++;
                }
                int len = i - start;
                char palavra[len + 1];
                strncpy(palavra, &frase[start], len);
                palavra[len] = '\0';
                
                // Cria átomo na cabeça
                aux->lista.no.cabeca = criaT(palavra);
                i--;  // Ajuste para compensar o incremento
            }
            i++;
        }
    }
    return inicio;
}