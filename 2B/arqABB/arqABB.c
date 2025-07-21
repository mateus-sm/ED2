#include <stdio.h>
#include <windows.h>

struct gravar {
    int info, esq, dir;
};
typedef struct gravar Gravar;

#define LER(ptr, arq) fread(ptr, sizeof(*(ptr)), 1, arq)
#define ESCREVER(ptr, arq) fwrite(ptr, sizeof(*(ptr)), 1, arq)
#define POS_ATUAL(arq) (ftell(arq) / sizeof(Gravar))
#define GOTO_POS(pos, arq) fseek(arq, pos * sizeof(Gravar), 0)
#define FIM(arq) fseek(arq, 0, 2)

void criaNoABB(Gravar *rec, int info) {
    rec->info = info;
    rec->esq = rec->dir = -1;
}

void insereABB(Gravar rec) {
    FILE *ptr = fopen("arqABB.dat", "rb+");
    Gravar atual;
    int flag, pos;

    FIM(ptr);
    int TF = POS_ATUAL(ptr);
    rewind(ptr);

    if (TF != 0) {
        LER(&atual, ptr);
        flag = 0;

        do {
            if (rec.info > atual.info) {
                if (atual.dir == -1) {
                    pos = POS_ATUAL(ptr) - 1;

                    FIM(ptr);
                    ESCREVER(&rec, ptr);

                    atual.dir = POS_ATUAL(ptr) - 1;

                    GOTO_POS(pos, ptr);
                    ESCREVER(&atual, ptr);

                    flag = 1;
                } else {
                    GOTO_POS(atual.dir, ptr);
                    LER(&atual, ptr);
                }
            } else {
                if (atual.esq == -1) {
                    pos = POS_ATUAL(ptr) - 1;

                    FIM(ptr);
                    ESCREVER(&rec, ptr);

                    atual.esq = POS_ATUAL(ptr) - 1;

                    GOTO_POS(pos, ptr);
                    ESCREVER(&atual, ptr);

                    flag = 1;
                } else {
                    GOTO_POS(atual.esq, ptr);
                    LER(&atual, ptr);
                }
            }
        } while (!flag);
    } else {
        ESCREVER(&rec, ptr);
    }

    fclose(ptr);
}

void exibirArq() {
    FILE *ptr = fopen("arqABB.dat", "rb");
    Gravar rec;

    while (LER(&rec, ptr)) {
        printf("| %2d | %2d | %2d |\n", rec.esq, rec.info, rec.dir);
    }

    fclose(ptr);
}

int main(void) {
    FILE *ptr = fopen("arqABB.dat", "wb");
    fclose(ptr);

    Gravar rec;

    criaNoABB(&rec, 10); insereABB(rec);
    criaNoABB(&rec, 12); insereABB(rec);
    criaNoABB(&rec, 11); insereABB(rec);
    criaNoABB(&rec, 14); insereABB(rec);
    criaNoABB(&rec, 8);  insereABB(rec);
    criaNoABB(&rec, 6);  insereABB(rec);
    criaNoABB(&rec, 9);  insereABB(rec);

    exibirArq();
    system("pause");
    return 0;
}