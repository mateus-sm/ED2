#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

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

struct bits {   
    unsigned char b7:1;   
    unsigned char b6:1;   
    unsigned char b5:1;   
    unsigned char b4:1;   
    unsigned char b3:1;   
    unsigned char b2:1;   
    unsigned char b1:1;   
    unsigned char b0:1; 
};  

union byte {   
    struct bits bi;   
    unsigned char num;
};

typedef union byte Byte;

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
void completarCodigos(Huff *arv, ListR *lista);
void codigo(Huff *arv, ListR *lista, int*, char*);

//Struct Gravar
Gravar *criaNoGravar(int simbolo, char *palavra, char *cod);
//Gravar
void gravarListaR(ListR *lista);
void exibirGravar();

void preencheBin(char *codigo, char *palavra, int *i);
void codificar(char *frase, char *codigo);

//Conferir tamanho do texto 
//Funçoes Exibir Arvore e Recuperar codigos retificar

void gravarCodificacao(char *cod);

int main(void) {
    ListR *lista = NULL;
    Forest *forest = NULL;
    Huff *arv = NULL;
    char bin[50], codificacao[100];
    int n, k;

    char *frase = "o rato roeu a roupa do rei de roma ";
    char *frase2 = "o rato roeu a roupa do rei de roma rato rato rei";
    //printf("Frase: \n\"%s\"\n\n", frase);
    char* texto =
        "o rato roeu a roupa do rei de roma "
        "de roma o rei o rato a roupa roeu "
        "roeu o rato a roupa de roma "
        "a roupa de roma o rato roeu "
        "o rei roeu o rato de roma "
        "o rato de roma o rei roeu "
        "o rei roeu o roupa do rato de roma "
        "de roma o rato a roupa roeu o rei "
        "a roupa roeu o rato do rei de roma "
        "do rei de roma o rato a roupa roeu "
        "a roupa roeu o rei do rato de roma "
        "do rato de roma o rei a roupa roeu "
        "a roupa roeu o rei o rato roeu a roma a roma roeu o rato "
        "o rato roeu o rei a roupa roeu a roma o rei roeu a roupa "
        "a roma roeu a roupa o rei roeu o rato a roupa roeu a roma "
        "o rei roeu a roma a roma roeu o rato o rato roeu a roupa "
        "o rato roeu a roma a roupa roeu o rei a roma roeu a roupa "
        "roeu o rei roeu o rato roeu a roma "
        "a roma o rato o rei roeu "
        "rato roeu roupa roeu rei roeu roma roeu "
        "o rato a roupa o rei a roma "
        "o rato rei roeu a roupa roma "
        "o rato rei roeu a roupa do rato de roma "
        "rato roeu roma roma roeu rato "
        "o rei roeu roma o rei roeu rato";
    //printf("Texto: \n\"%s\"\n\n", texto);
    
    construirListaRegistros(&lista, texto);
    //exibirListR(lista);

    criarFloresta(&forest, lista);
    //exibirFloresta(forest);

    gerarArvoreHuffman(&arv, &forest);
    n = -1;
    exibeHuff(arv, lista, &n);

    k = -1;
    codigo(arv, lista, &k, bin);

    exibirListR(lista);

    gravarListaR(lista);
    exibirGravar();

    //Teste de função
    // char codigo[50];
    // char *palavra = "rato";
    // int z = 0;
    // preencheBin(codigo, palavra, &z);
    // codigo[z] = '\0';
    // printf("codigo = %s\n", codigo); 
    // printf("palavra = %s\n", palavra); 
    // printf("index = %d\n", z);
    
    codificar(frase, codificacao);
    printf("Frase = %s\n", frase);
    printf("Codificacao: \n%s\n", codificacao);

    gravarCodificacao(codificacao);
    lerCodificacao();
    printf("\nArquivo: \n");

    system("pause");
    return 0;
}

void lerCodificacao() {
    FILE *ptr = fopen("Codificacao.dat", "rb");
    Byte B;

    fread(&(B.num), sizeof(Byte), 1, ptr);
    while(!feof(ptr)) {
        printf("%d", B.bi.b7);
        printf("%d", B.bi.b6);
        printf("%d", B.bi.b5);
        printf("%d", B.bi.b4);
        printf("%d", B.bi.b3);
        printf("%d", B.bi.b2);
        printf("%d", B.bi.b1);
        printf("%d", B.bi.b0);
        fread(&(B.num), sizeof(Byte), 1, ptr);
    }
}

void gravarCodificacao(char *cod) {
    FILE *ptr = fopen("Codificacao.dat", "wb");
    Byte B;
    int TL = strlen(cod);
    //printf("tamanho = %d\n", TL); system("pause");

    for (int i = 0; i < TL;) {
        B.bi.b7 = cod[i++];
        B.bi.b6 = cod[i++];
        B.bi.b5 = cod[i++];
        B.bi.b4 = cod[i++];
        B.bi.b3 = cod[i++];
        B.bi.b2 = cod[i++];
        B.bi.b1 = cod[i++];
        B.bi.b0 = cod[i++];

        fwrite(&(B.num), sizeof(Byte), 1, ptr);
        //printf("Byte em dec = %d\n", B.num);
    }

    fclose(ptr);
}

void preencheBin(char *codigo, char *palavra, int *i) {
    FILE *ptr = fopen("ListaRegistros.dat", "rb");
    Gravar record;
    int TL;

    if (ptr != NULL) {
        fread(&record, sizeof(Gravar), 1, ptr);
        while(!feof(ptr) && !(strcmp(palavra, record.palavra) == 0)) {
            fread(&record, sizeof(Gravar), 1, ptr);
        }

        if (strcmp(palavra, record.palavra) == 0) {
            TL = strlen(record.cod);
            for (int j = 0; j < TL; j++) {
                codigo[(*i)++] = record.cod[j];
            }
        }
    }

    fclose(ptr);
}

void codificar(char *frase, char *codigo) {
    char palavraLida[50];
    int TL = strlen(frase), k = 0, b = 0;

    for (int i = 0; i <= TL; i++) {
        if (frase[i] == ' ' || frase[i] == '\0') {
            if (k > 0) {
                palavraLida[k] = '\0';
                preencheBin(codigo, palavraLida, &b);
                k = 0;
            }
            if (frase[i] == ' ') {
                preencheBin(codigo, " ", &b);
            }
        } else {
            palavraLida[k++] = frase[i];
        }
    }
    //Preparar para gravar em bytes
    codigo[b] = '\0';
    TL = strlen(codigo);
    while ((TL % 8) != 0) {
        preencheBin(codigo, " ", &b);
        codigo[b] = '\0';
        TL = strlen(codigo);
    }

    codigo[b] = '\0';
}

Gravar *criaNoGravar(int simbolo, char *palavra, char *cod) {
    Gravar *record = (Gravar*)malloc(sizeof(Gravar));
    record->simbolo = simbolo;
    strcpy(record->palavra, palavra);
    strcpy(record->cod, cod);
    return record;
}

void gravarListaR(ListR *lista) {
    FILE *ptr = fopen("ListaRegistros.dat", "wb");
    Gravar *record = NULL;

    while (lista != NULL) {
        record = criaNoGravar(lista->simbolo, lista->palavra, lista->cod);
        fwrite(record, sizeof(Gravar), 1, ptr);
        free(record);
        lista = lista->prox;
    }

    fclose(ptr);
}

void exibirGravar() {
    FILE *ptr = fopen("ListaRegistros.dat", "rb");
    Gravar record;
    char palavra[50];

    if (ptr != NULL) {
        printf("Lista em disco: \n");
        printf("%-15s %10s %10s\n", "Palavra", "Simbolo", "Codigo");

        fread(&record, sizeof(Gravar), 1, ptr);
        while (!feof(ptr)) {
            sprintf(palavra, "\"%s\"", record.palavra);
            printf("%-15s %10d %10s\n", palavra, record.simbolo, record.cod);
            fread(&record, sizeof(Gravar), 1, ptr);
        }

    }

    fclose(ptr);
}

//Iterativo, nao ta pegando
void completarCodigos(Huff *arv, ListR *lista) {
    Huff *raiz = arv;
    ListR *auxList = NULL;
    char bin[50];
    int k = -1, aux = 1;
    Pilha P;

    inicializaPilha(&P);

    while(raiz != NULL || !pilhaVazia(P)) {
        if (raiz == NULL) {
            pop(&P, &raiz);
            k--;
            bin[k] = '1';
            raiz = raiz->dir;
            k++;
            if(raiz == NULL) {
                k = k - aux;
                aux = 1;
            } else {
                aux++;
            }
        } else {
            if ((raiz->esq == NULL) && (raiz->dir == NULL)) {
                auxList = lista;
                while (auxList != NULL && auxList->simbolo != raiz->simbolo) {
                    auxList = auxList->prox;
                }
                bin[++k] = '\0';
                k--;
                strcpy(auxList->cod, bin);
            }

            push(&P, raiz);
            k++;
            bin[k] = '0';
            raiz = raiz->esq;
        }
    }

}

void codigo(Huff *arv, ListR *lista, int *k, char *bin) {
    Huff *raiz = arv;
    ListR *auxList = NULL;

    if (arv != NULL) {
        if (raiz->esq == NULL && raiz->dir == NULL) {
            auxList = lista;
            while (auxList != NULL && auxList->simbolo != raiz->simbolo) {
                auxList = auxList->prox;
            }
            bin[(*k) + 1] = '\0';
            strcpy(auxList->cod, bin);
        } else {
            (*k)++;
            bin[(*k)] = '0'; 
            codigo(arv->esq, lista, k, bin);
            bin[(*k)] = '1'; 
            codigo(arv->dir, lista, k, bin);
            (*k)--;
        }
    }
}

char *retornaPalavra(ListR *lista, int simb) {
    while(lista != NULL && lista->simbolo != simb) {
        lista = lista->prox;
    }

    if (lista != NULL && lista->simbolo == simb) {
        return lista->palavra;
    }

    return "0";
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
        free(forest);
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
            printf("(0,%d)\n", raiz->frequencia);
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
    char palavra[50];

    if (atual != NULL) {
        printf("Lista de Registros: \n");
        printf("%-15s %10s %10s %10s\n", "Palavra", "Simbolo", "Freq", "Codigo");

        while (atual != NULL) {
            sprintf(palavra, "\"%s\"", atual->palavra);
            printf("%-15s %10d %10d %10s\n", palavra, atual->simbolo, atual->freq, atual->cod);
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