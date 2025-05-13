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
    int cod;
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

//Struct ListR
ListR *criaNoListaReg(int simbolo, char *palavra, int freq, int cod);
void inserirListR(ListR **lista, int simbolo, char *palavra, int freq, int cod);
void exibirListR(ListR *lista);
//lista de registros
void construirListaRegistros(ListR **lista, char *frase);
bool palavraCadastrada(char *palavra, ListR *lista);
int contaFrequencia(char *palavra, char *frase);

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
void exibeHuff(Huff *raiz);

//Outros
int getBits(int n);

//Gravar lista de registros com o ponteiro da mesma?
//Usar a lista de registro com freq e simb para fazer a arvore 
//  e só depois conseguir os codigos para a lista?
//Os codigos serão CTF ou CTV? 
//Ordem crescente ou decrescente faz diferença?
//Algoritmo de Huff ta correto?

int main(void) {
    ListR *lista = NULL;
    Forest *forest = NULL;
    Huff *arv = NULL;

    char *frase1 = " o tempo perguntou pro tempo quanto tempo o tempo tem "
                  "o tempo respondeu pro tempo que tempo tem o tempo ";
    printf("Frase: \n\"%s\"\n\n", frase1);
    char *frase2 = " o tempo perguntou pro tempo quanto tempo o tempo tem "
                  "o tempo respondeu pro tempo que o tempo o tempo tem ";
    //printf("Frase: \"%s\"\n\n", frase2);

    construirListaRegistros(&lista, frase1);
    exibirListR(lista);

    criarFloresta(&forest, lista);
    exibirFloresta(forest);

    gerarArvoreHuffman(&arv, &forest);
    exibeHuff(arv);

    system("pause");
    return 0;
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
            minF1 = NULL;
            free(minF2);
            minF2 = NULL;

            inserirFloresta(&(*forest), arv);
        }

        *raiz = (*forest)->tree;        
    }

}

void exibeHuff(Huff *raiz) {
    static int n = -1;
    if (n == -1) {
        printf("Arvore de Huffman: \n");
    }
    if (raiz != NULL) {
        n++;
        exibeHuff(raiz->dir);
        for (int i = 0; i < 5 * n; i++) { printf(" "); }
        printf("(%d,%d)\n", raiz->simbolo, raiz->frequencia);
        exibeHuff(raiz->esq);
        n--;
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

int getBits(int n) {
    //log de n(elementos diferentes em palavra)
    int i = 1;
    for(; n > pow(2,i);) {
        i++;
    }
    return i;
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

ListR *criaNoListaReg(int simbolo, char *palavra, int freq, int cod) {
    ListR *novo = (ListR*)malloc(sizeof(ListR));

    novo->simbolo = simbolo;
    strcpy(novo->palavra, palavra);
    novo->freq = freq;
    novo->cod = cod;
    novo->prox = NULL;

    return novo;
}

void inserirListR(ListR **lista, int simbolo, char *palavra, int freq, int cod) {
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
    printf("Lista de Registros: \n");
    while (atual != NULL) {
        printf("\"%s\" ", atual->palavra);
        printf("Simbolo: %d\n", atual->simbolo);
        printf("Frequencia: %d ", atual->freq);
        printf("Codigo: %d\n\n", atual->cod);
        atual = atual->prox;
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

                    inserirListR(lista, simbolo++, palavra, freq, 0);
                }

                k = 0;
            }
        } else {
            palavra[k++] = frase[i];
        }
    }

    if (espacos > 0) {
        inserirListR(lista, simbolo++, " ", espacos, 0);
    }
}