#include <stdio.h>
#include <math.h>

//#define DEBUG

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

Exp *criaNo(char terminal, Info info);
void insereNo(Exp *e, Exp *caixa);
void exibir(Exp *l);
void gerarListaGen(Exp **e, char *str);
double resolveFunc(char *func);
char plana(Exp *e);
void resolveZero(Exp *e);
void resolveExp(Exp *e);

int main(void) {
    //Casos:
    // Pode ser um numero
    // Pode ser um sinal + - * /
    // Pode ser um parenteses ( )
    // Pode ser uma função
    //     Função contem virgula
    char *expressao = "2+(3-5*(pow(2,3)-3)-8)"; //Esperado = -28
    Exp *e = NULL;
    
    gerarListaGen(&e, expressao);
    exibir(e);
    //printf("Resultado: %lf\n", resolveFunc("sqrt(8)")); system("pause");
    
    resolveExp(e);
    e->terminal = 'V';
    exibir(e);
    puts("");
    
    char *expressao2 = "2+3*(5-abs(7-9))+pow(2,3)-sqrt(9)+4*(6/3)"; //Esperado = 24
    gerarListaGen(&e, expressao2);
    exibir(e);
    resolveExp(e);
    e->terminal = 'V';
    exibir(e);
    
    system("pause");
    return 0;
}

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
    printf("\n");
}


void gerarListaGen(Exp **e, char *str) {
    Info uni;
    Exp *caixa, *lista, *aux;
    Pilha p;
    char func[20];
    int j;
    inicializaPilha(&p);

    //Do jeito que a logica foi feita é necessairo que 
    //  nao se comece nenhuma lista de NULL.
    //Logo eu adiciono 0 + ao inicio das listas.
    uni.valor = 0;
    lista = criaNo('V', uni);
    uni.operador = '+';
    lista->cauda = criaNo('O', uni);
    *e = lista;
    lista = lista->cauda;
   
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
                uni.valor = 0;
                caixa = criaNo('V', uni);
                uni.operador = '+';
                caixa->cauda = criaNo('O', uni);
                lista->cabeca = caixa;
                lista = caixa->cauda;
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

double resolveFunc(char *func) {
    char nome[10];
    int i, j = 0;
    double x, y = 0;

    for (i = 0; func[i] != '('; i++) {
        nome[j++] = func[i];
    }

    nome[j] = '\0'; //printf("%s\n", nome); system("pause");
        
    x = func[++i] - '0';
    if (func[i + 1] != ')') {
        y = func[i + 2] - '0';
    }
    
    if (strcmp(nome, "pow") == 0) {
        return pow(x, y);
    }

    else if (strcmp(nome, "sqrt") == 0) {
        return sqrt(x - y);
    }

    else if (strcmp(nome, "abs") == 0) {
        return abs(x - y);
    }

    else if (strcmp(nome, "sin") == 0) {
        return sin(x - y);
    }

    else if (strcmp(nome, "cos") == 0) {
        return cos(x - y);
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
        aux->cabeca = NULL;

        #ifdef DEBUG
            exibir(e);
        #endif
    }
}

void resolveExp(Exp *e) {
    Exp *aux = e, *ant;
    Pilha p;
    inicializaPilha(&p);
    
    if (!plana(aux)) {
        resolveZero(aux);
    }

    #ifdef DEBUG
        exibir(e);
    #endif
    
    //Primeiro resolve-se as funçoes
    aux = e;
    while (aux != NULL) {
        if (aux->terminal == 'F') {
            aux->terminal = 'V';
            aux->info.valor = resolveFunc(aux->info.func);
            
            #ifdef DEBUG
                exibir(e);
            #endif
        }
        aux = aux->cauda;
    }

    //Multiplicações e divisoes
    ant = e;
    aux = ant->cauda;
    while (aux != NULL) {
        //printf("%c\n", aux->terminal);
        if (aux->terminal == 'O') {
            if (aux->info.operador == '*' || aux->info.operador == '/') {
                if (aux->info.operador == '/') {
                    ant->info.valor /= aux->cauda->info.valor;
                } else {
                    ant->info.valor *= aux->cauda->info.valor;
                }
                ant->cauda = aux->cauda->cauda;
                free(aux->cauda);
                free(aux);
                aux = ant->cauda;
                
                #ifdef DEBUG
                    exibir(e);
                #endif
            } else {
                ant = aux;
                aux = aux->cauda;
            }
        } else {
            ant = aux;
            aux = aux->cauda;
        }
    }

    //Somas e subtrações
    ant = e;
    aux = ant->cauda;
    while (aux != NULL) {
        if (aux->terminal =='O') { 
            if (aux->info.operador == '+' || aux->info.operador == '-') {
                if (aux->info.operador == '+') {
                    ant->info.valor += aux->cauda->info.valor;
                } else {
                    ant->info.valor -= aux->cauda->info.valor;
                }
                ant->cauda = aux->cauda->cauda;
                free(aux->cauda);
                free(aux);
                aux = ant->cauda; 

                #ifdef DEBUG
                    exibir(e);
                #endif
            } else {
                //Em teoria nao é pra ter operadores alem de + e -
            }
        } else {
            ant = aux;
            aux = aux->cauda;
        }
    }
}
