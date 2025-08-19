#include <stdio.h>
#include <math.h>

union reg_info {
    double valor;
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

void resolveZero(Exp *e);
void resolveExp(Exp *e);

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
                case 'V': printf("%.2lf", l->info.valor); break;
                case 'O': printf("%c ", l->info.operador); break;
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


void gerarListaGen(Exp **e, char *str) {
    Info uni;
    Exp *caixa, *lista, *aux;
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
                insereNo(lista, caixa);
                lista = lista->cauda;
                push(&p, lista);
                
                //Adicionar sublista
                uni.operador = '+';
                caixa = criaNo('O', uni);
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

// int pow(int x, double y) {
//     int ret = 1;

//     for (int k = 0; k < y; k++) {
//         ret *= x;
//     }

//     return ret;
// }

// Pode capturar X e Y desse jeito?
// O valor é pra ser int ou double
// Pode fazer retorno por if
// É pra contemplar mais de uma casa decimal?
double resolveFunc(char *func) {
    char nome[10];
    int i, j = 0;
    double x, y, ret;

    for (i = 0; func[i] != '('; i++) {
        nome[j++] = func[i];
    }

    nome[j] = '\0'; //printf("%s\n", nome); system("pause");
        
    x = func[++i] - '0';
    y = func[i + 2] - '0';
    
    if (strcmp(nome, "pow") == 0) {
        return pow(x, y);
    }

    if (strcmp(nome, "sqrt") == 0) {
        return sqrt(x);
    }

    if (strcmp(nome, "abs") == 0) {
        return abs(x);
    }

    if (strcmp(nome, "sin") == 0) {
        return sin(x);
    }

    if (strcmp(nome, "cos") == 0) {
        return cos(x);
    }
}

char plana(Exp *e) {
    char flag = 1;

    while (e != NULL) {
        if (e->cabeca != NULL) {
            flag = 0;
        }
        e = e->cauda;
    }

    return flag;
}

void resolveZero(Exp *e) {
    Exp *aux = e;
    Pilha p, p2;
    inicializaPilha(&p);
    inicializaPilha(&p2);

    //Capturar cabeças de sublistas
    while (aux != NULL || !pilhaVazia(&p)) {
        if (aux == NULL){
            pop(&p, &aux);
            aux = aux->cauda;
        } else {
            push(&p, aux);
            if (aux->cabeca != NULL) {
                push(&p2, aux);
            }
            aux = aux->cabeca;
        }
    }
    
    while (!pilhaVazia(&p2)) {
        pop(&p2, &aux); //printf("Teste: "); printf("%c -> %.2lf\n", aux->terminal, aux->info.valor);
        aux->terminal = 'V';
        resolveExp(aux->cabeca);
        aux->info.valor = aux->cabeca->info.valor;
        //printf("%.2lf", aux->info.valor); getchar();
        free(aux->cabeca);
    }
}

void resolveExp(Exp *e) {
    Exp *aux = e, *ant;
    Pilha p;
    inicializaPilha(&p);
    
    if (!plana(aux)) {
        resolveZero(aux);
    }
    
    ant = aux;
    aux = aux->cauda;
    while (aux != NULL) {
        //printf("%c\n", aux->terminal);
        switch (aux->terminal) {
            case 'O': // Precisa resolver para operadores em sequencia??
                switch (aux->info.operador) {
                    case '*':
                        ant->info.valor *= aux->cauda->info.valor;
                        ant->cauda = aux->cauda->cauda;
                        free(aux->cauda);
                        free(aux);
                        ant = aux;
                        aux = ant->cauda; 
                    break;

                    case '/':
                        ant->info.valor /= aux->cauda->info.valor;
                        ant->cauda = aux->cauda->cauda;
                        free(aux->cauda);
                        free(aux);
                        ant = aux;
                        aux = ant->cauda;
                    break;

                    default:
                        ant = aux;
                        aux = aux->cauda;
                    break;
                }
            break;

            case 'F':
                aux->terminal = 'V';
                aux->info.valor = resolveFunc(aux->info.func);
                //printf("%.2lf", aux->info.valor); getchar();
            break;

            default:
                ant = aux;
                aux = aux->cauda;
            break;
        }
    }

    ant = e;
    aux = ant->cauda;
    printf("%.2lf", aux->info.valor); getchar(); //AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    while (aux != NULL) {
        switch (aux->terminal) {
            case 'O': // Precisa resolver para operadores em sequencia??
                switch (aux->info.operador) {
                    case '+':
                        ant->info.valor += aux->cauda->info.valor;
                        ant->cauda = aux->cauda->cauda;
                        free(aux->cauda);
                        free(aux);
                        ant = aux;
                        aux = ant->cauda; 
                    break;
                    
                    case '-':
                        ant->info.valor -= aux->cauda->info.valor;
                        ant->cauda = aux->cauda->cauda;
                        free(aux->cauda);
                        free(aux);
                        ant = aux;
                        aux = ant->cauda; 
                    break;
                }
            break;

            default:
                ant = aux;
                aux = aux->cauda;
            break;

            // case 'V':
            //     resultado += aux->info.valor;
            //     free(aux);
            //     pop(&p, &aux);
            //     //printf("%.2lf", resultado); getchar();
            // break;
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
    //printf("Resultado: %lf\n", resolveFunc("sqrt(8)")); system("pause");
    e->terminal = 'V';
    resolveExp(e);
    exibir(e);

    system("pause");

    return 0;
}