typedef struct strdin {
    char letra;
    struct strdin *prox;
} Strdin;

void inicializar(Strdin **ptr);
void reiniciar(Strdin **ptr);
void exibir(Strdin **str);
void ribixe(Strdin **ptr);
void imprimirAoContrario(Strdin **ptr);
void tamanho(Strdin **ptr, int *size);
void inserir(Strdin **str, char info);
void copia(Strdin **str1, Strdin **str2);
void concat(Strdin **str1, Strdin **str2, Strdin **str3);
void removeSelecao(Strdin **str, int nro, int start);

void inserirSub(Strdin **str, const char sub[]) {
    
}

void concat(Strdin **str1, Strdin **str2, Strdin **str3) {
    Strdin *aux, *ant;
    
    copia(str1, str3);

    if (*str3 == NULL) {
        *str3 = *str2;
    } else {
        aux = *str3;
    
        while (aux->prox != NULL) {
            aux = aux->prox;
        }
    
        aux->prox = *str2;
    }
}

// void concat(Strdin **str1, Strdin **str2, Strdin **str3) {
//     Strdin *aux3 = *str1;

//     while (aux3->prox != NULL) {
//         aux3 = aux3->prox;
//     }
//     aux3->prox = *str2;

//     copia(str1, str3);

//     aux3->prox = NULL;
// }

void copia(Strdin **str1, Strdin **str2) {
    Strdin *aux = *str1;
    reiniciar(str2);

    while(aux != NULL) {
        inserir(str2, aux->letra);
        aux = aux->prox;
    }
}

void tamanho(Strdin **ptr, int *size) {
    Strdin *aux = *ptr;

    while (aux != NULL) {
        aux = aux->prox;
        (*size)++;
    }
}

void ribixe(Strdin **ptr) {
    if (*ptr != NULL) {
        ribixe(&(*ptr)->prox);
        printf("[%c] ", (*ptr)->letra);
    }
}

void imprimirAoContrario(Strdin **ptr) {
    ribixe(ptr);
    puts("");
}

void reiniciar(Strdin **ptr) {
    Strdin *aux, *atual;

    if (*ptr != NULL) {
        atual = *ptr;
        while(atual != NULL) {
            aux = atual;
            atual = atual->prox;
            free(aux);
        }

        *ptr = NULL;
    }
}

void inicializar(Strdin **ptr) {
    *ptr = NULL;
}

void removeSelecao(Strdin **str, int nro, int start) {
    Strdin *ant = *str, *aux, *atual;
    int qtd = 0;

    if (ant != NULL) {
        //chegar no lugar desejado
        while (qtd < start - 1 && ant != NULL) {
            ant = ant->prox;
            qtd++;
        }

        //excluir
        if (ant != NULL) {
            if (start == 0) {
                atual = ant;
                while (nro > 0 && atual != NULL) {
                    aux = atual;
                    atual = atual->prox;
                    free(aux);
                    nro--;
                }
                *str = atual;
            }
            
            else {
                while (nro > 0 && ant->prox != NULL) {
                    aux = ant->prox;
                    ant->prox = aux->prox;
                    free(aux);
                    nro--;
                }
            }
        }
    }
}

void inserir(Strdin **str, char info) {
    Strdin *aux, *atual;
    aux = (Strdin *)malloc(sizeof(Strdin));
    aux->letra = info;
    aux->prox = NULL;

    if (*str == NULL) {
        *str = aux;
    } else {
        atual = *str;
        while(atual->prox != NULL) {
            atual = atual->prox;
        }

        atual->prox = aux;
    }
}

void exibir(Strdin **str) {
    Strdin *aux;

    if (*str != NULL) {
        aux = *str;
        while(aux->prox != NULL) {
            printf("[%c] ", aux->letra);
            aux = aux->prox;
        }

        printf("[%c]\n", aux->letra);
    } else {
        printf("NULL\n");
    }
}