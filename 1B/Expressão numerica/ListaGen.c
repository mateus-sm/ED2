#include <stdio.h>

union reg_info {
    int valor;
    char operador;
    char func[20];
};

typedef union reg_info Info;

struct exp {
    char terminal;
    union reg_info info;
    struct exp *cabeca;
    struct exp *cauda; 
};

typedef struct exp Exp;

#include "pilha.h"

Exp *criaNo(char terminal, Info info) {
    Exp *aux = (Exp*)malloc(sizeof(Exp));

    aux->terminal = terminal;
    aux->info = info;
    aux->cabeca = NULL;
    aux->cauda = NULL;

    return aux;
}

void insereNo(Exp *e, Exp *caixa) {
    Exp *aux = e;
    
    if (e == NULL) {
        e = caixa;
    } else {
        while (aux->cauda != NULL) {
            aux = aux->cauda;
        }

        aux->cauda = caixa;
    }
}

void exibir(Exp* l) {
    Pilha P;
    inicializaPilha(&P);

    printf("[");
    while (l != NULL || !pilhaVazia(&P)) {
        if (l != NULL) {
            switch (l->terminal) {
                case 'V': printf("%d", l->info.valor); break;
                case 'O': printf("%c", l->info.operador); break;
                case 'F': printf("%s", l->info.func); break;
            }
            push(&P, l);
            l = l->cabeca;
            if (l != NULL) {
                printf("[");
            }
        } else { 
            pop(&P, &l);
            l = l->cauda;
            if (l != NULL) {
                printf(", ");   
            } else {
                printf("]");
            }      
        }

    }
    printf("]\n");
}


void gerarListaGen(Exp **e, char * str) {
    Info uni;
    Exp *caixa, *lista;
    Pilha p;
    char func[20];
    int j;
    inicializaPilha(&p);

    uni.operador = '+';
    lista = criaNo('O', uni);
    *e = lista;
   
    for (int i = 0; str[i] != '\0'; i++) {

        switch (str[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
                uni.operador = str[i];
                caixa = criaNo('O', uni);
                insereNo(lista, caixa);
                lista = lista->cauda;
            break;

            case '(':
                uni.valor = 0;
                caixa = criaNo('V', uni);
                push(&p, lista);
                lista = lista->cabeca = caixa;
            break;
            
            case ')':
                pop(&p, &lista);
            break;

            default:
                if (str[i] >= '0' && str[i] <= '9') {
                    //Numero
                    uni.valor = str[i] - 48;
                    caixa = criaNo('V', uni);
                    lista = lista->cauda = caixa;
                } else {
                    //Função
                    j = 0;
                    while(str[i] != ')') {
                        func[j++] = str[i];
                        i++;
                    }
                    func[j++] = ')';
                    func[j] = '\0';

                    strcpy(uni.func, func);
                    caixa = criaNo('F', uni);
                    lista = lista->cauda = caixa;
                }
            break;
        }

    }
}

int main(void) {
    //Casos:
    // Pode ser um numero
    // Pode ser um sinal + - * /
    // Pode ser um parenteses ( )
    // Pode ser uma função
    //     Função contem virgula
    char *expressao = "2+(3-5*(pow(2,3)-3)-8)";
    Exp *e = NULL;

    gerarListaGen(&e, expressao);
    exibir(e);

    system("pause");

    return 0;
}