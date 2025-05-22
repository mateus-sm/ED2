#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct huff {
    struct huff *esq;
    int      simbolo;
    int   frequencia;
    struct huff *dir;
};

typedef struct huff Huff;

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

//Struct Huffman
Huff *criaNoHuff(int freq, int simb);

//Huffman
void montarHuff(Huff **raiz);
void insereHuff(Huff *raiz, char *cod, int simb);
void exibeHuff(Huff *raiz, int *n);
void decodificaHuff(Huff *arv, char *codificacao, char *frase);

//Arquivo/String
void retornaPalavra(int simb, char *palavra);
void lerCodificacao(char *);
int procuraSimbolo(Huff *arv, char *cod);


int main(void) {
    Huff *arv = NULL;
    char codificacao[100];
    char frase[100];
    int n;

    montarHuff(&arv);
    n = -1;
    exibeHuff(arv, &n);

    lerCodificacao(codificacao);
    printf("Arquivo: %s\n", codificacao);

    decodificaHuff(arv, codificacao, frase);
    printf("Frase: \"%s\"\n", frase);

    system("pause");
    return 0;
}

int procuraSimbolo(Huff *arv, char *cod) {
    Huff *raiz = arv;

    for (int i = 0; i < strlen(cod); i++) {
        arv = (cod[i] == '1') ? arv->dir : arv->esq;
    }

    return arv->simbolo;
}

void decodificaHuff(Huff *arv, char *codificacao, char *frase) {
    char cod[50], palavra[50];
    int k = 0, simb;

    frase[0] = '\0';
    for (int i = 0; i < strlen(codificacao); i++) {
        cod[k++] = codificacao[i];
        cod[k] = '\0';

        simb = procuraSimbolo(arv, cod);

        if (simb != 0) {
            retornaPalavra(simb, palavra);
            sprintf(frase, "%s%s", frase, palavra);
            k = 0;
            palavra[0] = '\0';
        }
    }
}

void montarHuff(Huff **raiz) {
    Gravar record;
    FILE *ptr = fopen("ListaRegistros.dat", "rb");

    if (*raiz == NULL) {
        *raiz = criaNoHuff(0, 0);
    }

    fread(&record, sizeof(Gravar), 1, ptr);
    while(!feof(ptr)) {
        insereHuff(*raiz, record.cod, record.simbolo);
        fread(&record, sizeof(Gravar), 1, ptr);
    }

    fclose(ptr);
}

void insereHuff(Huff *raiz, char *cod, int simb) {
    int TL = strlen(cod);

    for (int i = 0; i < TL; i++) {
        if (cod[i] == '1') {
            if (raiz->dir == NULL) {
                raiz->dir = criaNoHuff(0, 0);
            }

            raiz = raiz->dir;
        } 
        else {
            if (raiz->esq == NULL) {
                raiz->esq = criaNoHuff(0, 0);
            }

            raiz = raiz->esq;
        }
    }

    raiz->simbolo = simb;
}

void retornaPalavra(int simb, char *palavra) {
    Gravar record;
    FILE *ptr = fopen("ListaRegistros.dat", "rb");

    fread(&record, sizeof(Gravar), 1, ptr);
    while(!feof(ptr) && !(record.simbolo == simb)) {
        fread(&record, sizeof(Gravar), 1, ptr);
    }

    if (record.simbolo == simb) {
        strcpy(palavra, record.palavra);
    } else {
        strcpy(palavra, "0");
    }

    fclose(ptr);
}


Huff *criaNoHuff(int freq, int simb) {
    Huff *arv = (Huff*)malloc(sizeof(Huff));

    arv->esq = NULL;
    arv->dir = NULL;
    arv->simbolo = simb;
    arv->frequencia = freq;

    return arv;
}

void exibeHuff(Huff *raiz, int *n) {
    char palavra[50];

    if (raiz != NULL) {
        if ((*n) == -1) {
            printf("Arvore de Huffman: \n");
        }
        (*n)++;
        exibeHuff(raiz->dir, n);
        for (int i = 0; i < 5 * (*n); i++) { printf(" "); }
        if (raiz->simbolo == 0) {
            printf("(0,%d)\n", raiz->frequencia);
        } else {
            retornaPalavra(raiz->simbolo, palavra);
            printf("(\"%s\",%d)\n", palavra, raiz->frequencia);
        }
        exibeHuff(raiz->esq, n);
        (*n)--;
    }
}

void lerCodificacao(char *aux) {
    FILE *ptr = fopen("Codificacao.dat", "rb");
    Byte B;
    int i = 0;

    fread(&(B.num), sizeof(Byte), 1, ptr);
    while(!feof(ptr)) {
        aux[i++] = B.bi.b0 + 48;
        aux[i++] = B.bi.b1 + 48;
        aux[i++] = B.bi.b2 + 48;
        aux[i++] = B.bi.b3 + 48;
        aux[i++] = B.bi.b4 + 48;
        aux[i++] = B.bi.b5 + 48;
        aux[i++] = B.bi.b6 + 48;
        aux[i++] = B.bi.b7 + 48;
        fread(&(B.num), sizeof(Byte), 1, ptr);
    }
    aux[i] = '\0';

    fclose(ptr);
}