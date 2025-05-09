#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct listaRegistros {
    int simbolo;
    char palavra[50];
    int freq;
    int cod;
    struct listaRegistros *prox;
};

typedef struct listaRegistros ListR;

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
    int count = 0, palavras = 0;
    char palavraLida[50];

    for(int i = 0, k = 0; i <= strlen(frase); i++) {
        if ((frase[i] == ' ' || frase[i] == '\0') && palavraLida[0] != '\0') { //Delimitador
            palavraLida[k++] = '\0'; //printf("palavra lida = %s\n", palavraLida);

            if (strcmp(palavraLida, palavra) == 0) {
                palavras++;
            }
            
            palavraLida[0] = '\0';
            k = 0;
            i++;
        }
        palavraLida[k++] = frase[i];
    }

    return palavras;
}


void construirListaRegistros(ListR **lista) {
    char *frase = "amar e sonhar sonhar e viver viver e curtir curtir e amar"; //printf("%s\n", frase);
    char palavra[20];
    int freq = 0, simbolo = 0;
    ListR *lr = NULL;

    palavra[0] = '\0';
    for (int i = 0, k = 0; i <= strlen(frase); i++) {
        if (frase[i] == ' ' && palavra[0] != '\0') { //Atingi o Delimitador
            palavra[k++] = '\0'; //printf("palavra = %s\n", palavra);

            if (!palavraCadastrada(palavra, *lista)) {
                freq = contaFrequencia(palavra, frase);
                //printf("%s tem freq = %d\n", palavra, freq);

                lr = (ListR*)malloc(sizeof(ListR));
            }

            palavra[0] = '\0';
            k = 0;
            i++;
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