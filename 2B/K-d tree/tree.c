#include <stdio.h>
#include <windows.h>
#define K 2
#define TF 6

struct kdtree {
    int ponto[K];
    struct kdtree *esq;
    struct kdtree *dir;
};

typedef struct kdtree Kdtree;

// void insereR(Kdtree **raiz, int ponto[K], int n) {
//     int d;
//     if (*raiz == NULL) {
//         *raiz = Cria_no(ponto);
//     }

//     else {
//         d = n % K;
//         if (ponto[d] < (*raiz)->ponto[d])
//             insereR(&(*raiz)->esq, ponto, n + 1);
//         else
//             insereR(&(*raiz)->dir, ponto, n + 1);
//     }
// }

//Insere balanceado
//Ordenar pelo X
//Pegar o elemento do meio e tornar raiz
//Inverter a ordenação e repetir

void ordena(int pontos[TF][K], int d, int ini, int fim) {
    int TL = fim, i, j, aux[K];
    while (TL > ini) {
        for (i = ini; i < TL; i++) {
            if (pontos[i][d] > pontos[i + 1][d]) {
                for (j = 0; j < K; j++) 
                    aux[j] = pontos[i][j];
                for (j = 0; j < K; j++) 
                    pontos[i][j] = pontos[i + 1][j];
                for (j = 0; j < K; j++) 
                    pontos[i + 1][j] = aux[j];
            }
        }

        TL--;
    }
}

Kdtree *CriaNo(int ponto[K]) {
    Kdtree *no = (Kdtree*)malloc(sizeof(Kdtree));
    int i;
    for (i = 0; i < K; i++) {
        no->ponto[i] = ponto[i];
    }
    no->esq = no->dir = NULL;
    return no;
}

void insereBalanceado(Kdtree **raiz, int pontos[TF][K], int n, int ini, int fim) {
    int d, meio;

    if(ini < fim) {
        d = n % K;
        ordena(pontos, d, ini, TF - 1);
        meio = (ini + fim) / 2;

        *raiz = CriaNo(pontos[meio]);
        insereBalanceado(&(*raiz)->esq, pontos, n + 1, ini, meio - 1);
        insereBalanceado(&(*raiz)->esq, pontos, n + 1, meio + 1, fim);
    }
}

int main(void) {
    int pontos[TF][K] = {{30,40}, {5,25}, {10,12}, {70,70}, {50,30}, {35,40}};
    int i;
    Kdtree *raiz = NULL;
    //insereBalanceado(&raiz, pontos, 0, 0, TF - 1);
    ordena(pontos, 0, 0, TF - 1);
    for (i = 0; i < TF; i++) {
        printf("%d,%d\n", pontos[i][0], pontos[0][i]);
    }

    system("pause");
    return 0;
}