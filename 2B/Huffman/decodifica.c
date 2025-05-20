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

void lerCodificacao();


int main(void) {
    //ListR *lista = NULL;
    //Forest *forest = NULL;
    //Huff *arv = NULL;
    //char;
    //int;

    printf("Arquivo: \n");
    lerCodificacao();

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