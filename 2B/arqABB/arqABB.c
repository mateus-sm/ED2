#include <stdio.h>
#include <windows.h>

struct gravar {
    int info, esq, dir;
};

typedef struct gravar Gravar;

void criaNoABB(Gravar *rec, int info) {
    (*rec).info = info;
    (*rec).esq = (*rec).dir = -1;
}

void insereABB(Gravar rec) {
    FILE *ptr = fopen("arqABB.dat", "rb+");
    Gravar atual;
    int flag, pos;

    fseek(ptr, 0, 2);
    int TF = ftell(ptr) / sizeof(Gravar); 
    //printf("TF = %d\n", TF); system("pause");
    rewind(ptr);
    
    if (TF != 0) {
        //printf("Arq nao vazio!\n");
        fread(&atual, sizeof(Gravar), 1, ptr);
        
        flag = 0;
        do  {
            if (rec.info > atual.info) {
                if (atual.dir == -1) {
                    //Guarda pos atual
                    pos = (ftell(ptr) / sizeof(Gravar)) - 1;

                    //Grava no fim
                    fseek(ptr, 0, 2);
                    fwrite(&rec, sizeof(Gravar), 1, ptr);

                    //Atualiza pos do fim
                    atual.dir = (ftell(ptr) / sizeof(Gravar)) - 1;

                    //Volta pra pos e grava a atualização
                    fseek(ptr, pos * sizeof(Gravar), 0);
                    fwrite(&atual, sizeof(Gravar), 1, ptr);

                    flag = 1;
                } else {
                    fseek(ptr, atual.dir * sizeof(Gravar), 0);
                    fread(&atual, sizeof(Gravar), 1, ptr);
                }
            } else {
                if (atual.esq == -1) {
                    pos = (ftell(ptr) / sizeof(Gravar)) - 1;

                    fseek(ptr, 0, 2);
                    fwrite(&rec, sizeof(Gravar), 1, ptr);

                    atual.esq = (ftell(ptr) / sizeof(Gravar)) - 1;

                    fseek(ptr, pos * sizeof(Gravar), 0);
                    fwrite(&atual, sizeof(Gravar), 1, ptr);
                    flag = 1;
                } else {
                    fseek(ptr, atual.esq * sizeof(Gravar), 0);
                    fread(&atual, sizeof(Gravar), 1, ptr);
                }
            }
        } while (flag == 0);
    } else {
        //printf("Arq vazio!\n");
        fwrite(&rec, sizeof(Gravar), 1, ptr);
    }

    fclose(ptr);
}

void exibirArq() {
    FILE *ptr = fopen("arqABB.dat", "rb");
    Gravar rec;

    fread(&rec, sizeof(Gravar), 1, ptr);
    while(!feof(ptr)) {
        printf("| %2d | %2d | %2d |\n", rec.esq, rec.info, rec.dir);
        fread(&rec, sizeof(Gravar), 1, ptr);
    }

    fclose(ptr);
}

int main (void) {
    FILE *ptr = fopen("arqABB.dat", "wb");
    fclose(ptr);

    Gravar rec;

    criaNoABB(&rec, 10);
    insereABB(rec);
    criaNoABB(&rec, 12);
    insereABB(rec);
    criaNoABB(&rec, 11);
    insereABB(rec);
    criaNoABB(&rec, 14);
    insereABB(rec);
    criaNoABB(&rec, 8);
    insereABB(rec);
    criaNoABB(&rec, 6);
    insereABB(rec);
    criaNoABB(&rec, 9);
    insereABB(rec);

    exibirArq();

    system("pause");
    return 0;
}