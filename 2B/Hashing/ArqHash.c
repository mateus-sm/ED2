#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define N 13

typedef struct reg {
    int numero;
    char nome[10];
    double salario;
    int elo;
    char status;
} Reg;

Reg inserirDados(int numero, char *nome, double salario) {
    Reg reg;
    reg.numero = numero;
    strcpy(reg.nome, nome);
    reg.salario = salario;
    reg.elo = 0;
    reg.status = 'T';

    return reg;
}

void criaArquivoHash(char *nome) {
    FILE *arq = fopen(nome, "rb");
    Reg reg = inserirDados(0, "", 0);
    if (arq == NULL) {
        arq = fopen(nome, "ab");
        for (int i = 0; i < N; i++) {
            fwrite(&reg, sizeof(reg), 1, arq);
        }
    }

    fclose(arq);
}

int Hash(int chave) {
    return chave % N;
}

int fileSize(FILE *arq) {
    fseek(arq, 0, 2);
    return ftell(arq) / sizeof(Reg);
}

char buscaHash(FILE *arq, int numero, int ender) {
    Reg reg;

    fseek(arq, ender * sizeof(Reg), 0);
    fread(&reg, sizeof(Reg), 1, arq);
    while (numero != reg.numero && reg.elo != 0) {
        fseek(arq, reg.elo * sizeof(Reg), 0);
        fread(&reg, sizeof(Reg), 1, arq);
    }

    if (numero == reg.numero) {
        return 1;
    }

    return 0;
}

void insereHash(char *nome, Reg reg) {
    int ender;
    Reg regDad;
    FILE *arq = fopen(nome, "rb+");

    ender = Hash(reg.numero);
    fseek(arq, ender * sizeof(Reg), 0);
    fread(&regDad, sizeof(Reg), 1, arq);

    if (regDad.numero == 0) { //pos da area de dados vazia
        fseek(arq, ender * sizeof(Reg), 0);
        fwrite(&reg, sizeof(reg), 1, arq);
    }
    else { // colisão
        if (!buscaHash(arq, reg.numero, ender)) {
            reg.elo = regDad.elo;
            regDad.elo = fileSize(arq);

            fseek(arq, ender * sizeof(Reg), 0);
            fwrite(&regDad, sizeof(Reg), 1, arq);
            fseek(arq, 0, 2);
            fwrite(&reg, sizeof(Reg), 1, arq);
        }
    }

    fclose(arq);
}

void exibir(char *nomeArq) {
    int i = 0;
    Reg reg;
    FILE *ptr = fopen(nomeArq, "rb");
    fseek(ptr, 0, 0);

    printf("%3s | %4s | %-10s | %7s | %1s | %3s\n", "Pos", "Nro", "Nome", "Salario", "S", "Elo");
    printf("----+------+------------+---------+---+-----\n");
    while (fread(&reg, sizeof(Reg), 1, ptr)) {
        printf("%3d | %4d | %-10s | %7.2f | %c | %3d\n", i, reg.numero, reg.nome, reg.salario, reg.status, reg.elo);

        i++;
        if (i == N) { printf("----+------+------------+---------+---+-----\n"); }
    }

    fclose(ptr);
}

void excluir (char *nome, int chave) {
    int ender;
    Reg reg;
    FILE *arq = fopen(nome, "rb+");

    ender = Hash(chave);

    fseek(arq, ender * sizeof(Reg), 0);
    fread(&reg, sizeof(Reg), 1, arq);

    while (chave != reg.numero && reg.elo != 0) {
        ender = reg.elo;
        fseek(arq, ender * sizeof(Reg), 0);
        fread(&reg, sizeof(Reg), 1, arq);
    }

    if (chave == reg.numero) {
        reg.status = 'F';
        fseek(arq, ender * sizeof(Reg), 0);
        fwrite(&reg, sizeof(Reg), 1, arq);
    }

    fclose(arq);
}

void resolveEncadeamento(FILE *arq, Reg reg, int *pos) {
    //Possibilidades
    //Area de dados
    //Excluir reg com elo 0
    //Excluir reg com elo != 0
    //Overflow
    //Excluir reg com elo 0
    //Excluir reg na ultima pos
    //Excluir reg com elo != 0

    int ender, tl, chave;
    char status;
    Reg aux;

    if (reg.status == 'F') {
        //Excluir reg com elo 0
        if (reg.elo == 0) {
            aux = inserirDados(0, "", 0);
            fseek(arq, (*pos) * sizeof(Reg), 0);
            fwrite(&aux, sizeof(Reg), 1, arq);
        }
        else { //Excluir reg com elo != 0
            ender = reg.elo;
            chave = reg.numero;

            //Copia o reg ender para o reg
            fseek(arq, ender * sizeof(Reg), 0);
            fread(&aux, sizeof(Reg), 1, arq);
            status = aux.status;
            fseek(arq, (*pos) * sizeof(Reg), 0);
            fwrite(&aux, sizeof(Reg), 1, arq);

            tl = fileSize(arq) - 1;

            //Removido reg original da area de dados, tratar overflow
            //Se o reg original apontava para tl, basta fazer o ftruncate
            if (ender != tl) {
                //Substitui quem o elo apontava pelo ultimo reg
                fseek(arq, tl * sizeof(Reg), 0);
                fread(&reg, sizeof(Reg), 1, arq);
                chave = reg.numero;
                fseek(arq, ender * sizeof(Reg), 0);
                fwrite(&reg, sizeof(Reg), 1, arq);
            }

            ftruncate(fileno(arq), tl * sizeof(Reg));

            //Se o ender do reg original, apontar para tl, esse reg é o original
            //Caso contrario esse reg é o apontado do apontado pelo reg
            if (reg.elo != tl) {
                //Atualizar area de dados
                fseek(arq,Hash(chave) * sizeof(Reg), 0);
                fread(&aux, sizeof(Reg), 1, arq);
                aux.elo = ender;
                fseek(arq,Hash(chave) * sizeof(Reg), 0);
                fwrite(&aux, sizeof(Reg), 1, arq);
            }

            if (status == 'F') {
                (*pos)--;
            }
        }
    }
    else {

    }
}

void reorganiza(char *nome) {
    FILE *arq = fopen(nome, "rb+");
    Reg reg;

    for (int i = 0; i < N; i++) {
        fseek(arq, i * sizeof(Reg), 0);
        fread(&reg, sizeof(Reg), 1, arq);
        resolveEncadeamento(arq, reg, &i);
        //printf("%3d | %4d | %-10s | %7.2f | %c | %3d\n", i, reg.numero, reg.nome, reg.salario, reg.status, reg.elo);
    }

    fclose(arq);
}

int main(void) {
    criaArquivoHash("arqHash.dat");

    //Hash 0
    insereHash("arqHash.dat", inserirDados(1950, "Enio", 900));
    insereHash("arqHash.dat", inserirDados(2600, "Luis", 5000));
    //Hash 1
    insereHash("arqHash.dat", inserirDados(1600, "Eber", 800));
    //Hash 4
    insereHash("arqHash.dat", inserirDados(3150, "Rui", 550));
    //Hash 5
    insereHash("arqHash.dat", inserirDados(2150, "Flavio", 420));
    //Hash 7
    insereHash("arqHash.dat", inserirDados(1450, "Diogo", 1200));
    //Hash 8
    insereHash("arqHash.dat", inserirDados(1100, "Antonio", 750));
    //Hash 9
    insereHash("arqHash.dat", inserirDados(1400, "Claudio", 600));
    //Hash 10
    insereHash("arqHash.dat", inserirDados(1050, "Afonso", 3200));
    insereHash("arqHash.dat", inserirDados(2766, "Pedro", 3000));
    insereHash("arqHash.dat", inserirDados(3000, "Ivan", 630));
    insereHash("arqHash.dat", inserirDados(1700, "Edson", 500));
    //Hash 12
    insereHash("arqHash.dat", inserirDados(1000, "Ademar", 1200));
    insereHash("arqHash.dat", inserirDados(2300, "Gerson", 800));

    // insereHash("arqHash.dat", inserirDados(4, "TESTEELSE1", 1111));
    // insereHash("arqHash.dat", inserirDados(17, "TESTEELSE2", 2222));
    // insereHash("arqHash.dat", inserirDados(30, "TESTEELSE3", 3333));
    // insereHash("arqHash.dat", inserirDados(10, "TESTEIF4", 4444));
    // insereHash("arqHash.dat", inserirDados(23, "TESTEIF5", 5555));
    // insereHash("arqHash.dat", inserirDados(36, "TESTEIF6", 6666));

    exibir("arqHash.dat");
    puts("");

    //Exclusao Logica
    excluir("arqHash.dat", 1600);
    excluir("arqHash.dat", 1700);
    excluir("arqHash.dat", 1950);
    excluir("arqHash.dat", 2600);
    exibir("arqHash.dat");
    puts("");

    reorganiza("arqHash.dat");
    exibir("arqHash.dat");

    remove("arqHash.dat");

    //system("pause");
    return 0;
}