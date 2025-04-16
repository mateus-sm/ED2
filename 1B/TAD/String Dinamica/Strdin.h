#include <stdbool.h>
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
void inserirSub(Strdin **str, const char sub[], int start);
bool menor(Strdin **str1, Strdin **str2);

//13) Busca a posição local na string str1 em que a string subs se inicia. Se local = 0
//      então subs não está contida em str1, caso contrário local é a posição de início 
//      da string subs dentro da string str1
void buscaLocal(Strdin **str, int *local, const char sub[]) {
    Strdin *aux = *str;
    int flag = 0, j = 0, k, inicio = 0;

    while (aux != NULL) {
        if (aux->letra == sub[0]) {
            //verificar palavra completa
            inicio = j;
            for (k = 0; sub[k] != '\0', aux != NULL; k++) {
                if(aux->letra != sub[k]) {
                    flag = 1;
                }
                aux = aux->prox;
                j++;
            }

            //saiu antes de terminar o match com sub
            if (sub[k] != '\0') {
                flag = 1;
            }

            //condição de parada, evita demais matchs
            if (flag == 0) {
                aux = NULL;
            }
        } else {
            aux = aux->prox;
            j++;
        }
    }

    *local = (flag == 1) ? 0 : inicio;
}

//12) Função que verifica se uma string (str1) é igual a outra (str2), caso verdade a função 
//     retorna True, senão False;
bool equal(Strdin **str1, Strdin **str2) {
    Strdin *aux1 = *str1, *aux2 = *str2;
    int flag = 0;

    while (aux1 != NULL && aux2 != NULL) {
        if (aux1->letra != aux2->letra) {
            flag = 1;
        }

        aux1 = aux1->prox;
        aux2 = aux2->prox;
    }

    return (flag == 0) ? true : false;
}

//11)  Função  que  verifica  se  uma  string  str1  é  menor  que  outra  str2,  
//      caso  verdade  a função retorna True, senão False;
bool menor(Strdin **str1, Strdin **str2) {
    int x1 = 0, x2 = 0;

    tamanho(str1, &x1);
    tamanho(str2, &x2);

    return (x1 < x2) ? true : false;
}

//10) Insere na string str1 uma substring subs a partir da posição start;
void inserirSub(Strdin **str, const char sub[], int start) {
    Strdin *aux = *str, *inicio, *final;
    int i = 0;

    if (start > 0 && *str != NULL) {
        //Chegar na pos
        while (aux != NULL && i < start - 1) {
            aux = aux->prox;
            i++;
        }

        if (aux != NULL) {
            //criar fim
            final = aux->prox;
            aux->prox = NULL;

            //inserir string
            for (int j = 0; sub[j] != '\0'; j++) {
                inserir(&aux, sub[j]);
                aux = aux->prox;
            }

            //Conetar inicio ao fim
            aux->prox = final;
        }
    } 
    
    if (start == 0) {
        //Criar inicio
        aux = NULL;
        inserir(&aux, sub[0]);
        inicio = aux;

        //inserir string
        for (int j = 1; sub[j] != '\0'; j++) {
            inserir(&aux, sub[j]);
            aux = aux->prox;
        }
        
        //conectar inicio ao fim
        aux->prox = *str;
        *str = inicio;
    }
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
        printf("'%c' ", (*ptr)->letra);
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
            printf("'%c' ", aux->letra);
            aux = aux->prox;
        }

        printf("'%c'\n", aux->letra);
    } else {
        printf("NULL\n");
    }
}