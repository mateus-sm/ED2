#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct listaRegistros {
    int simbolo;
    char palavra[50];
    int freq;
    char cod[50];
    struct listaRegistros *prox;
};

typedef struct listaRegistros ListR;

struct huff {
    struct huff *esq;
    int      simbolo;
    int   frequencia;
    struct huff *dir;
};

typedef struct huff Huff;

struct forest {
    struct huff *tree;
    struct forest *prox;
};

typedef struct forest Forest;

struct gravar {
    int simbolo;
    char palavra[50];
    char cod[50];
};

typedef struct gravar Gravar;

#include "pilha.h"

//Struct ListR
ListR *criaNoListaReg(int simbolo, char *palavra, int freq, char *cod);
void inserirListR(ListR **lista, int simbolo, char *palavra, int freq, char *cod);
void exibirListR(ListR *lista);
//lista de registros
void construirListaRegistros(ListR **lista, char *frase);
bool palavraCadastrada(char *palavra, ListR *lista);
int contaFrequencia(char *palavra, char *frase);
char *retornaPalavra(ListR *lista, int simb);

//Struct Floresta
Forest *criaNoForest(Huff *arv);
void inserirFloresta(Forest **forest, Huff *arv);
void exibirFloresta(Forest *forest);
//Floresta
void criarFloresta(Forest **ptr, ListR *lista);

//Struct Huffman
Huff *criaNoHuff(int freq, int simb);

//Huffman
void gerarArvoreHuffman(Huff **raiz, Forest **forest);
void exibeHuff(Huff *raiz, ListR *, int *);

char *completarCodigos(Huff *arv, ListR *lista);

int main(void) {
    ListR *lista = NULL;
    Forest *forest = NULL;
    Huff *arv = NULL;
    int n;

    char *palavra = "abrac abrac abra";
    char *frase1 = " o tempo perguntou pro tempo quanto tempo o tempo tem "
                  "o tempo respondeu pro tempo que tempo tem o tempo ";
    //printf("Frase: \n\"%s\"\n\n", frase1);
    char *frase2 = " o tempo perguntou pro tempo quanto tempo o tempo tem "
                  "o tempo respondeu pro tempo que o tempo o tempo tem ";
    //printf("Frase: \n\"%s\"\n\n", frase2);
    char* texto =
        "o rato roeu a roupa do rei de roma de roma o rei o rato a roupa roeu "
        "roeu o rato a roupa de roma a roupa de roma o rato roeu "
        "o rei roeu o rato de roma o rato de roma o rei roeu "
        "o rei roeu o roupa de rato de roma de roma o rato a roupa roeu o rei "
        "a roupa roeu o rato do rei de roma do rei de roma o rato a roupa roeu "
        "a roupa roeu o rei do rato de roma do rato de roma o rei a roupa roeu "
        "a roupa roeu o rei o rato roeu a roma a roma roeu o rei "
        "roeu o rei roeu o rato roeu a roma "
        "a roma o rato o rei roeu "
        "rato roeu roma roma roeu rato o rei roeu roma o rei roeu rato";
    //printf("Texto: \n\"%s\"\n\n", texto);
    
    construirListaRegistros(&lista, texto);
    exibirListR(lista);

    criarFloresta(&forest, lista);
    //exibirFloresta(forest);

    gerarArvoreHuffman(&arv, &forest);
    n = -1;
    exibeHuff(arv, lista, &n);

    puts(" ");
    completarCodigos(arv, lista);
    puts(" ");

    exibirListR(lista);

    system("pause");
    return 0;
}

char *completarCodigos(Huff *arv, ListR *lista) {
    Huff *raiz = arv, *aux = NULL;
    ListR *auxList = NULL;
    char bin[50];
    int k = 0;
    Pilha P;

    inicializaPilha(&P);

    while(raiz != NULL || !pilhaVazia(P)) {
        if (raiz == NULL) {
            pop(&P, &raiz);
            raiz = raiz->dir;
            bin[k++] = '1';
        } else {
            if ((raiz->esq == NULL) && (raiz->dir == NULL)) {
                auxList = lista;
                while (auxList != NULL && auxList->simbolo != raiz->simbolo) {
                    auxList = auxList->prox;
                }
                bin[k] = '\0';
                strcpy(auxList->cod, bin);
                k = 0;
            }

            push(&P, raiz);
            raiz = raiz->esq;
            bin[k++] = '0';
        }
    }

}

char *retornaPalavra(ListR *lista, int simb) {
    static char espadas[2];
    sprintf(espadas, "%c", 5);

    while(lista != NULL && lista->simbolo != simb) {
        lista = lista->prox;
    }

    if (lista != NULL && lista->simbolo == simb) {
        return lista->palavra;
    } else {
        return espadas;
    }
}

void gerarArvoreHuffman(Huff **raiz, Forest **forest) {
    Forest *fr = NULL, *minF1 = NULL, *minF2 = NULL;
    Huff *arv = NULL, *minT1 = NULL, *minT2 = NULL;

    if (*forest != NULL) {
        while ((*forest)->prox != NULL) {
            minF1 = *forest;
            minT1 = minF1->tree;

            minF2 = (*forest)->prox;
            minT2 = minF2->tree;

            arv = criaNoHuff(minT1->frequencia + minT2->frequencia, 0);
            arv->esq = minT1;
            arv->dir = minT2;

            *forest = minF2->prox;
            free(minF1);
            free(minF2);

            inserirFloresta(&(*forest), arv);
        }

        *raiz = (*forest)->tree;        
    }

}

void exibeHuff(Huff *raiz, ListR *lista, int *n) {
    if (raiz != NULL) {
        if ((*n) == -1) {
            printf("Arvore de Huffman: \n");
        }
        (*n)++;
        exibeHuff(raiz->dir, lista, n);
        for (int i = 0; i < 5 * (*n); i++) { printf(" "); }
        if (raiz->simbolo == 0) {
            printf("%4s\n", retornaPalavra(lista, raiz->simbolo));
        } else {
            printf("(\"%s\",%d)\n", retornaPalavra(lista, raiz->simbolo), raiz->frequencia);
        }
        exibeHuff(raiz->esq, lista, n);
        (*n)--;
    }
}

Forest *criaNoForest(Huff *arv) {
    Forest *forest = (Forest*)malloc(sizeof(Forest));
    forest->tree = arv;
    forest->prox = NULL;
    return forest;
}

void inserirFloresta(Forest **forest, Huff *arv) {
    Forest *atual = *forest, *ant = NULL;

    Forest *fr = criaNoForest(arv);

    if (*forest == NULL) {
        (*forest) = fr;
    } else {
        while (atual != NULL && atual->tree->frequencia < arv->frequencia) {
            ant = atual;
            atual = atual->prox;
        }

        if (ant == NULL) {
            fr->prox = *forest;
            *forest = fr;
        } else {
            ant->prox = fr;
            fr->prox = atual;
        }
    }
}

void exibirFloresta(Forest *forest) {
    Huff *arv = NULL;
    printf("Floresta: \n");
    while (forest != NULL) {
        arv =  forest->tree;
        printf("Simbolo = %d ", arv->simbolo);
        printf("Frequencia = %d \n", arv->frequencia);
        printf("Esq = %p ", arv->esq);
        printf("Dir = %p \n\n", arv->dir);
        forest = forest->prox;
    }
}

Huff *criaNoHuff(int freq, int simb) {
    Huff *arv = (Huff*)malloc(sizeof(Huff));

    arv->esq = NULL;
    arv->dir = NULL;
    arv->simbolo = simb;
    arv->frequencia = freq;

    return arv;
}

void criarFloresta(Forest **ptr, ListR *lista) {
    Huff *arv = NULL;
    while(lista != NULL) {
        arv = criaNoHuff(lista->freq, lista->simbolo);
        inserirFloresta(ptr, arv);
        lista = lista->prox;
    }
}

bool palavraCadastrada(char *palavra, ListR *lista) {
    bool flag = false;

    while (lista != NULL) {
        if (strcmp(lista->palavra, palavra) == 0) {
            flag = true;
        }
        lista = lista->prox;
    }

    return flag;
}

int contaFrequencia(char *palavra, char *frase) {
    int palavras = 0, len = strlen(frase);
    char palavraLida[50];

    for (int i = 0, k = 0; i <= len; i++) {
        if (frase[i] == ' ' || frase[i] == '\0') { //Delimitador
            if (k > 0) {
                palavraLida[k++] = '\0'; //printf("palavra lida = %s\n", palavraLida); system("pause");

                if (strcmp(palavraLida, palavra) == 0) {
                    palavras++;
                }
                
                k = 0;
            }
        } else {
            palavraLida[k++] = frase[i];
        }
    }

    return palavras;
}

ListR *criaNoListaReg(int simbolo, char *palavra, int freq, char *cod) {
    ListR *novo = (ListR*)malloc(sizeof(ListR));

    novo->simbolo = simbolo;
    strcpy(novo->palavra, palavra);
    novo->freq = freq;
    strcpy(novo->cod, cod);
    novo->prox = NULL;

    return novo;
}

void inserirListR(ListR **lista, int simbolo, char *palavra, int freq, char *cod) {
    ListR *ant = NULL, *atual = *lista;
    ListR *no = criaNoListaReg(simbolo, palavra, freq, cod);

    if (*lista == NULL) {
        *lista = no;
    } else {
        while (atual != NULL && atual->freq < freq) {
            ant = atual;
            atual = atual->prox;
        }

        if (ant == NULL) {
            no->prox = *lista;
            *lista = no;
        } else {
            ant->prox = no;
            no->prox  = atual;
        }
    }
}

void exibirListR(ListR *lista) {
    ListR *atual = lista;

    if (atual != NULL) {
        printf("Lista de Registros: \n");
        printf("Palavra\tSimbolo\tFreq\tCodigo\n");

        while (atual != NULL) {
            printf("\"%s\"\t%d\t%d\t%s\n", atual->palavra, atual->simbolo, atual->freq, atual->cod);
            atual = atual->prox;
        }
    }
}

void construirListaRegistros(ListR **lista, char *frase) {
    char palavra[20];
    int freq = 0, simbolo = 1, espacos = 0;

    for (int i = 0, k = 0; i <= strlen(frase); i++) {
        if (frase[i] == ' ' || frase[i] == '\0') { //Atingi o Delimitador
            espacos += (frase[i] == ' ') ? 1 : 0;

            if (k > 0) { //Atingi o Delimitador e a palavra nao esta vazia
                palavra[k] = '\0'; //printf("palavra = %s\n", palavra); system("pause");

                if (!palavraCadastrada(palavra, *lista)) {
                    freq = contaFrequencia(palavra, frase);
                    //printf("%s tem freq = %d\n", palavra, freq);

                    inserirListR(lista, simbolo++, palavra, freq, "-");
                }

                k = 0;
            }
        } else {
            palavra[k++] = frase[i];
        }
    }

    if (espacos > 0) {
        inserirListR(lista, simbolo++, " ", espacos, "-");
    }
}