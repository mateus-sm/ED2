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

    //removeSelecao(&inicio, 2, 0);
    exibir(&inicio);

    //reiniciar(&inicio);
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

    if(menor(&str2, &str1)) {
        printf("Eh menor\n");
    }

    copia(&str1, &str2);
    exibir(&str2);

    if(equal(&str2, &str1)) {
        printf("Eh igual\n");
    }
    
    Strdin *str3 = NULL;
    concat(&str1, &str2, &str3);
    exibir(&str3);

    puts("");
    exibir(&inicio);
    // Teste 5: Inserção em posição fora dos limites (negativa)
    inserirSub(&inicio, "ERRO", -1);
    exibir(&inicio);

    int local = 0;
    buscaLocal(&inicio, &local, "DEDE");
    printf("' ' esta na pos: %d\n", local);

    system("pause");
    return 0;
}
