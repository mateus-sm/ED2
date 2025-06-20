#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definição das Estruturas ---

// Estrutura do Nó da Lista Generalizada
typedef struct Gen {
    int terminal; // 1 se for átomo (info), 0 se for lista (lista)
    union {
        char info[50];
        struct {
            struct Gen* cabeca; // head
            struct Gen* calda;  // tail
        } lista;
    } no;
} Gen;

// --- Implementação da Pilha de Ponteiros Gen* ---

typedef struct NoPilha {
    Gen* dado;
    struct NoPilha* prox;
} NoPilha;

typedef struct {
    NoPilha* topo;
} Pilha;

void inicializaPilha(Pilha* p) {
    p->topo = NULL;
}

void push(Pilha* p, Gen* ptr) {
    NoPilha* novo = (NoPilha*)malloc(sizeof(NoPilha));
    novo->dado = ptr;
    novo->prox = p->topo;
    p->topo = novo;
}

int pop(Pilha* p, Gen** ptr) {
    if (p->topo == NULL) {
        return 0; // Pilha vazia
    }
    NoPilha* aux = p->topo;
    *ptr = aux->dado;
    p->topo = aux->prox;
    free(aux);
    return 1;
}

// --- Função para Construir a Lista a partir da String (Refatorada) ---
// Ex: "[x, a, [c, d], [e, a, [f]], b]" ou "[]"
Gen* construirLista(const char* str) {
    if (strcmp(str, "[]") == 0) {
        return NULL;
    }

    Gen *raiz, *l, *aux;
    Pilha p;
    inicializaPilha(&p);
    char info[50];
    int i = 1, j;
    int len = strlen(str);

    // Cria o nó raiz da lista principal
    raiz = (Gen*)malloc(sizeof(Gen));
    raiz->terminal = 0;
    raiz->no.lista.cabeca = NULL;
    raiz->no.lista.calda = NULL;
    l = raiz;

    while (i < len - 1) {
        switch (str[i]) {
            case ' ':
                i++; // Ignora espaços
                continue;

            case '[':
                // Início de uma sublista
                aux = (Gen*)malloc(sizeof(Gen));
                aux->terminal = 0;
                aux->no.lista.cabeca = NULL;
                aux->no.lista.calda = NULL;

                push(&p, l); // Salva o ponteiro da lista pai

                l->no.lista.cabeca = aux; // Conecta a sublista como cabeça
                l = aux; // Desce para o nível da sublista
                break;

            case ']':
                // Fim de uma sublista
                pop(&p, &l); // Retorna para o nível da lista pai
                break;

            case ',':
                // Separador de elementos, prepara a cauda
                aux = (Gen*)malloc(sizeof(Gen));
                aux->terminal = 0;
                aux->no.lista.cabeca = NULL;
                aux->no.lista.calda = NULL;
                
                l->no.lista.calda = aux; // Conecta o novo nó como cauda
                l = l->no.lista.calda; // Move o ponteiro para a cauda
                break;

            default:
                // É um átomo
                j = 0;
                while (str[i] != ',' && str[i] != ']' && str[i] != ' ') {
                    info[j++] = str[i++];
                }
                info[j] = '\0';
                i--; // Ajusta o índice para o loop principal

                aux = (Gen*)malloc(sizeof(Gen));
                aux->terminal = 1;
                strcpy(aux->no.info, info);

                l->no.lista.cabeca = aux; // Conecta o átomo como cabeça
                break;
        }
        i++;
    }

    return raiz;
}

// --- Função para Exibir a Lista (para verificação) ---

void exibirLista(Gen* l) {
    if (l == NULL) {
        return;
    }

    printf("[");
    Gen* temp = l;
    while (temp != NULL) {
        if (temp->no.lista.cabeca != NULL) {
            if (temp->no.lista.cabeca->terminal == 1) {
                printf("%s", temp->no.lista.cabeca->no.info);
            } else {
                exibirLista(temp->no.lista.cabeca);
            }
        }
        
        temp = temp->no.lista.calda;
        if (temp != NULL) {
            printf(", ");
        }
    }
    printf("]");
}

// --- Função Principal para Teste ---

int main() {
    char str[] = "[x, a, [c, d], [e, a, [f]], b]";
    printf("String original: %s\n", str);

    Gen* lista = construirLista(str);

    printf("Lista construida: ");
    exibirLista(lista);
    printf("\n");

    // Liberação da memória (essencial em projetos reais)
    // freeList(lista); 
    system("pause");
    return 0;
}