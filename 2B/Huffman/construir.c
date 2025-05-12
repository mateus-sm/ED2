#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct listaRegistros {
    int simbolo;
    char palavra[50];
    int freq;
    int cod;
    struct listaRegistros *prox;
};

typedef struct listaRegistros ListR;

int getBits(int n) {
    //log de n(elementos diferentes em palavra)
    int i = 1;
    for(; n > pow(2,i);){
        i++;
    }
    return i;
}

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

ListR *criaNoListaReg(int simbolo, char *palavra, int freq, int cod) {
    ListR *novo = (ListR*)malloc(sizeof(ListR));

    novo->simbolo = simbolo;
    strcpy(novo->palavra, palavra);
    novo->freq = freq;
    novo->cod = cod;
    novo->prox = NULL;

    return novo;
}

void inserir(ListR **lista, int simbolo, char *palavra, int freq, int cod) {
    ListR *ant = NULL, *atual = *lista;
    ListR *no = criaNoListaReg(simbolo, palavra, freq, cod);

    if (*lista == NULL) {
        *lista = no;
    } else {
        // 1) percorre até achar o ponto de inserção
        while (atual != NULL && atual->freq < freq) {
            ant = atual;
            atual = atual->prox;
        }

        // 2) insere no início se ant == NULL
        if (ant == NULL) {
            no->prox = *lista;
            *lista   = no;
        }
        // 3) ou insere entre ant e atual
        else {
            ant->prox = no;
            no->prox  = atual;
        }
    }
}

void exibir(ListR *lista) {
    ListR *atual = lista;
    while (atual != NULL) {
        printf("\"%s\" ", atual->palavra);
        printf("Simbolo: %d\n", atual->simbolo);
        printf("Frequencia: %d ", atual->freq);
        printf("Codigo: %d\n\n", atual->cod);
        atual = atual->prox;
    }
}

void construirListaRegistros(ListR **lista, char *frase) {
    char palavra[20];
    int freq = 0, simbolo = 0, espacos = 0;
    ListR *lr = NULL;

    palavra[0] = '\0';
    for (int i = 0, k = 0; i <= strlen(frase); i++) {
        if (frase[i] == ' ' || frase[i] == '\0') {
            espacos += (frase[i] == ' ') ? 1 : 0;
            i++;

            if (palavra[0] != '\0') { //Atingi o Delimitador e a palavra nao esta vazia
                palavra[k++] = '\0'; 
                //printf("palavra = %s\n", palavra);

                if (!palavraCadastrada(palavra, *lista)) {
                    freq = contaFrequencia(palavra, frase);
                    //printf("%s tem freq = %d\n", palavra, freq);

                    inserir(lista, simbolo++, palavra, freq, 0);
                }

                palavra[0] = '\0';
                k = 0;
            }
        }
        
        palavra[k++] = frase[i];
    }

    if (espacos > 0) {
        inserir(lista, simbolo++, " ", espacos, 0);
    }
}

int main(void) {
    ListR *lista = NULL, *aux = NULL;
    char x;
    char *frase = " amar e sonhar sonhar e viver viver e curtir curtir e amar cada um tera "
                  "uma escolha cada um fara a escolha cada um escolhe a sua escolha levaremos "
                  "um tempo para crescer levaremos um tempo para amadurecer levaremos um tempo "
                  "para entender levaremos um tempo para envelhecer levaremos um tempo para morrer"; //printf("%s\n", frase);

    construirListaRegistros(&lista, frase);

    exibir(lista);

    system("pause");
    return 0;
}