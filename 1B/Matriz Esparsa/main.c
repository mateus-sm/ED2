#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct linhacoluna {
	char info;
	struct linhacoluna *prox;
	struct celula *prim;
};

typedef struct linhacoluna LinCol;

struct descritor {
    LinCol *plinha, *pcoluna;
};

typedef struct descritor TpDescritor;

struct celula {
	int linha, coluna;
	char info[50];
	struct celula *lin, *col;
};

typedef struct celula TpCelula;
