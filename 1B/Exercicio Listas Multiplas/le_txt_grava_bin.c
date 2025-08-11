#include <stdio.h>
#include <windows.h>
#include <string.h>

#define LER(reg, arq, st) fread(&reg, sizeof(st), 1, arq);
#define ESCREVER(reg, arq, st) fwrite(&reg, sizeof(st), 1, arq);

typedef struct {
	char autores[100], titulo_livro[100], editora[50];
	int ano, paginas;
} Reg;

void imprime() {
	FILE *arq = fopen("livros.dat", "rb");
    Reg reg;

    LER(reg, arq, Reg);//fread(&reg, sizeof(Reg), 1, arq);
    while (!feof(arq)) {	
    	printf("%s\n%s\n%s\t%d\t%d\n\n", reg.autores, reg.titulo_livro, reg.editora, reg.ano, reg.paginas);
        LER(reg, arq, Reg);//fread(&reg, sizeof(Reg), 1, arq);
    }
}

//1)
void gera_arq_bin() {
	FILE *arq = fopen("livros.txt", "r");
    Reg reg;
    FILE *arqBin = fopen("livros.dat", "wb");
    
    fscanf(arq, "%[^|]|%[^|]|%[^|]|%d|%d", &reg.autores, &reg.titulo_livro, &reg.editora, &reg.ano, &reg.paginas);
    while(!feof(arq)){
        //printf("%s\t%s\t%s\t%d\t%d\n", reg.autores, reg.titulo, reg.editora, reg.ano, reg.pag );
    	ESCREVER(reg, arqBin, Reg); //fwrite(&reg, sizeof(Reg), 1, arqBin);
    	fscanf(arq, "%[^|]|%[^|]|%[^|]|%d|%d", &reg.autores, &reg.titulo_livro, &reg.editora, &reg.ano, &reg.paginas);
    };

    fclose(arq);
    fclose(arqBin);
    printf("Arquivo binario gerado!\n\n");	
}

//2)
typedef struct editora {
    char editora[50];
    struct livro *plivro;
    struct editora *prox;
} Editora;

typedef struct livro {
    char *titulo;
    int ano;
    int paginas;
    struct listaAutor *pListaAutor;
    struct livro *prox;
    struct livro *ant;
} Livro;

typedef struct listaAutor {
    struct autor *PAutor;
    struct listaAutor *prox;
} ListaAutor;

typedef struct autor {
    char *sobrenome;
    char *nome;
    struct autor *prox;
} Autor;

void inserirEditora(Editora **ptr, Editora *aux);
Editora* criarEditora(char *editora);
void criaListaEditora(Editora **);

int main(void) {
    Editora *all = NULL;

    //1)
	gera_arq_bin();
	imprime();

    //2) 
    criaListaEditora(&all);

    while (all != NULL) {
        printf("%s\n", all->editora);
        all = all->prox;
    }
    system("pause");
	return 0;
}

void inserirEditora(Editora **ptr, Editora *no) {
    Editora *atual = NULL;

    if (*ptr == NULL) {
        *ptr = no;
    } else {
        atual = *ptr;
        while (atual->prox != NULL) {
            atual = atual->prox;
        }
        
        atual->prox = no;
    }
}

Editora* criarEditora(char *editora) {
    Editora *aux = (Editora*)malloc(sizeof(Editora));
    strcpy(aux->editora, editora);
    aux->plivro = NULL;
    aux->prox = NULL;
    return aux;
}


void criaListaEditora(Editora **ptr) {
    FILE *arq = fopen("livros.dat", "rb");
    Reg reg;

    LER(reg, arq, Reg);
    while(!feof(arq)) {
        inserirEditora(ptr, criarEditora(reg.editora));

        LER(reg, arq, Reg);
    }
}
