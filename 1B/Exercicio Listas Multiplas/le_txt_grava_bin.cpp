#include <stdio.h>
#include <windows.h>

struct TpRegistro  {
	char autores[100], titulo_livro[100], editora[50];
	int ano, paginas;
};

void imprime() {
	FILE *arqBin = fopen("livros.dat", "rb");
    TpRegistro reg;

    fread(&reg, sizeof(TpRegistro), 1, arqBin);
    while (!feof(arqBin)) {	
    	printf("%s\n%s\n%s\t%d\t%d\n\n", reg.autores, reg.titulo_livro, reg.editora, reg.ano, reg.paginas);
        fread(&reg, sizeof(TpRegistro), 1, arqBin);
    }
}

//1)
void gera_arq_bin() {
	FILE *arq = fopen("livros.txt", "r");
    TpRegistro reg;
    FILE *arqBin = fopen("livros.dat", "wb");
    
    fscanf(arq, "%[^|]|%[^|]|%[^|]|%d|%d", &reg.autores, &reg.titulo_livro, &reg.editora, &reg.ano, &reg.paginas);
    while(!feof(arq)){
        //printf("%s\t%s\t%s\t%d\t%d\n", reg.autores, reg.titulo, reg.editora, reg.ano, reg.pag );
    	fwrite(&reg, sizeof(TpRegistro), 1, arqBin);
    	fscanf(arq, "%[^|]|%[^|]|%[^|]|%d|%d", &reg.autores, &reg.titulo_livro, &reg.editora, &reg.ano, &reg.paginas);
    };

    fclose(arq);
    fclose(arqBin);
    printf("\n\nConcluido\n");	
}

//2)
struct Editora {
    char editora[];
    Livro *plivro;
    Editora *prox;
};

struct Livro {
    char titulo[];
    int ano;
    int paginas;
    ListaAutor *pListaAutor;
    Livro *prox;
    Livro *ant;
};

struct ListaAutor {
    Autor *PAutor;
    ListaAutor *prox;
};

struct Autor {
    char sobrenome[];
    char nome[];
    Autor *prox;
};

void criaListaEditora(Editora **);

int main(void) {
    //1)
	gera_arq_bin();
	imprime();

    //2) 

    system("pause");
	return 0;
}

void criaListaEditora(Editora **ptr) {

}
