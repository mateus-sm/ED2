#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct listaRegistros {
    int simbolo;
    char palavra[50];
    int freq;
    int cod;
    struct listaRegistros *prox;
};

typedef struct listaRegistros ListR;

bool palavraCadastrada(char *palavra, ListaR *lista) {
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
    int count = 0, palavras = 0;

    for(int i = 0, k = 0; frase[i] != '\0'; i++) {
        
    }

    return palavras;
}


void construirListaRegistros(ListR **lista) {
    char *frase = " amar e sonhar sonhar e viver viver e curtir curtir e amar."; //printf("%s\n", frase);
    char palavra[20];
    int freq = 0, simbolo = 0;


    for (int i = 0, k = 0; frase[i] != '\0', i++) {
        if (!isalpha(frase[i]) || frase[i] == '\0') { //Caracter nao é letra
            if (palavra[0] != '\0') { //Foi capturada uma palavra
                
                if (!palavraCadastrada(palavra, lista)) {
                    freq = contaFrequencia(palavra, frase);

                }

            }
        }
        palavra[k++] = frase[i];
    }
}

int main(void) {
    ListR *lista = NULL;

    construirListaRegistros(&lista);


    system("pause");
    return 0;
}