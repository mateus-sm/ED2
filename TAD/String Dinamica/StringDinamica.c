#include <stdio.h>
#include <stdlib.h>

#include "Strdin.h"

int main(void) {
    Strdin *inicio;
    inicializar(&inicio);
    
    inserir(&inicio, 'A');
    inserir(&inicio, 'u');
    inserir(&inicio, 'l');
    inserir(&inicio, 'a');
    inserir(&inicio, ' ');
    inserir(&inicio, 'd');
    inserir(&inicio, 'e');
    inserir(&inicio, ' ');
    inserir(&inicio, 'E');
    inserir(&inicio, 'D');

    int x = 0;
    tamanho(&inicio, &x);
    printf("Tamanho da string eh: %d\n", x);

    exibir(&inicio);
    imprimirAoContrario(&inicio);

    removeSelecao(&inicio, 2, 0);
    exibir(&inicio);

    reiniciar(&inicio);
    exibir(&inicio);

    puts("");
    Strdin *str1 = NULL;
    inserir(&str1, 'G');
    inserir(&str1, 'a');
    inserir(&str1, 't');
    inserir(&str1, 'o');

    Strdin *str2 = NULL;
    inserir(&str2, 'C');
    inserir(&str2, 'a');
    inserir(&str2, 't');

    copia(&str1, &str2);
    exibir(&str2);

    Strdin *str3 = NULL;
    concat(&str1, &str2, &str3);
    exibir(&str3);

    system("pause");
    return 0;
}
