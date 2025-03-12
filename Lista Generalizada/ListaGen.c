#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct reg_lista {
    struct listagen *cabeca;
    struct listagen *calda;
};

union info_lista {
    char info[8];
    struct reg_lista lista;
};

struct listagen {
    int terminal;
    union info_lista no;
};

typedef struct listagen ListaGen;



int main(void) {
    ListaGen *L = NULL;

    L = (ListaGen*)malloc(sizeof(ListaGen));

    L->terminal = 1;
    strcpy(L->no.info, "aaa");

    printf("%d\n", L->terminal);
    printf("%s\n", L->no.info);

    system("pause");
    return 0;
}

