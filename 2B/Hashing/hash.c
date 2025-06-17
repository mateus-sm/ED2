#include <stdio.h>
#include <windows.h>

#define N 5

struct caixa {
    int chave, pos;
    struct caixa *prox;
};

typedef struct caixa Caixa;

struct reg {
    int cod;
    char str[50];
    char status;
};

typedef struct reg Reg;

int Hash(int X) {
    return X % N;
}

Caixa *buscaLista(Caixa *caixa, int cod, Caixa **retorno) {
    while (caixa != NULL && !caixa->chave == cod) {
        caixa = caixa->prox;
    }

    if (caixa->chave == cod) {
        *retorno = caixa;
    } else {
        *retorno = NULL;
    }
}

void insereLista(Caixa **caixa, Reg reg, int pos) {
    Caixa *aux = *caixa;

    if (*caixa == NULL) {
        *caixa = (Caixa*)malloc(sizeof(Caixa));
        (*caixa)->chave = Hash(reg.cod);
        (*caixa)->pos = pos;
        (*caixa)->prox = NULL;
    } else {
        while(aux != NULL) {
            aux = aux->prox;
        }

        aux = (Caixa*)malloc(sizeof(Caixa));
        aux->chave = Hash(reg.cod);
        aux->pos = pos;
        aux->prox = NULL;
    }
}

void insereHash(Caixa T[], Reg reg) {
    FILE *ptr = fopen("ArqHash.dat", "ab+");
    int ender, pos;
    Caixa *aux;

    ender = Hash(reg.cod);
    aux = buscaLista(&(T[ender]), reg.cod, &aux);

    if (aux == NULL) {
        fseek(ptr, 0, 2);
        pos = ftell(ptr) / sizeof(Reg);
        insereLista(&(T[ender]), reg, pos);
        fwrite(&reg, sizeof(Reg), 1, ptr);
    } else {
        printf("Não encontrado!\n");
    }

    fclose(ptr);
}

int main(void) {
    Caixa T[N];
    Reg reg;
    reg.cod = 10;
    strcpy(reg.str, "mateus");
    reg.status = 1;

    insereHash(T, reg);
    system("pause");

    printf("%d", T[Hash(reg.cod)].chave);

    //Função do meio do quadrado
    // unsigned int chave = 225, v1;
    // v1 = chave << 23;
    // chave = v1 >> 29;

    // printf("%d\n", chave);

    system("pause");
    return 0;
}