#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>

#define N 13

struct tpreg{
    int numero, elo;
    char nome[50], status;
    float salario;
};
typedef struct tpreg TpReg;

int Hash(int numero){
    return numero % 13;
}

//construir a área de dados ou espalhamento
void criaArquivoHash(char nomeArq[]){
    FILE *ptr = fopen(nomeArq, "rb");
    TpReg reg;
    int i;
    if(ptr == NULL){
        ptr = fopen(nomeArq, "wb");
        reg.numero = 0;
        strcpy(reg.nome, "\0");
        reg.salario = 0.0;
        reg.status = '0';
        reg.elo = 0;
        for(i = 0; i < N; i++)
            fwrite(&reg, sizeof(TpReg), 1, ptr);
    }

    fclose(ptr);
}

TpReg inserirDados(int numero, char *nome, float salario){
    TpReg reg;
    reg.numero = numero;
    strcpy(reg.nome, nome);
    reg.salario = salario;
    reg.elo = 0;
    reg.status = 'T';

    return reg;
}

void buscaHash(FILE *ptr, int numero, int ender, char *achou){
    TpReg reg;
    fseek(ptr, ender * sizeof(TpReg), 0);
    fread(&reg, sizeof(TpReg), 1, ptr);
    while(numero != reg.numero && reg.elo != 0){
        fseek(ptr, reg.elo *sizeof(TpReg), 0);
        fread(&reg, sizeof(TpReg), 1, ptr);
    }
    if(numero == reg.numero)
        *achou = 1;
    else
        *achou = 0;
}


void insereHash(char nomeArq[], TpReg reg){
    int ender;
    char achou;
    TpReg regDad;
    FILE *ptr = fopen(nomeArq, "rb+");

    ender = Hash(reg.numero);
    fseek(ptr, ender*sizeof(TpReg), 0);
    fread(&regDad, sizeof(TpReg), 1, ptr);
    //posicao livre
    if(regDad.numero == 0){                   
        fseek(ptr, ender*sizeof(TpReg), 0);
        fwrite(&reg, sizeof(TpReg), 1, ptr);
    } 
    else{ //colisao
        buscaHash(ptr, reg.numero, ender, &achou);
        if(!achou){
            reg.elo = regDad.elo;
            fseek(ptr, 0, 2);
            regDad.elo = ftell(ptr)/sizeof(TpReg);
            fwrite(&reg,sizeof(TpReg), 1, ptr);
            fseek(ptr, ender *sizeof(TpReg),0);
            fwrite(&regDad,sizeof(TpReg), 1, ptr);
        }
    }
    fclose(ptr);
}

void excluir(char nomeArq[], int chave){
    int ender;
    TpReg reg;
    FILE *ptr = fopen(nomeArq, "rb+");

    ender = Hash(chave);

    fseek(ptr, ender *sizeof(TpReg), 0);
    fread(&reg, sizeof(TpReg), 1, ptr);
    
    while(reg.numero != chave && reg.elo != 0){
        ender = reg.elo;
        fseek(ptr, ender *sizeof(TpReg), 0);
        fread(&reg, sizeof(TpReg), 1, ptr);
    }
        
    if(reg.numero == chave){
        reg.status = 'F';
        fseek(ptr, ender *sizeof(TpReg), 0);
        fwrite(&reg, sizeof(TpReg), 1, ptr);
    }
    
    fclose(ptr);
}

void exibir(char nomeArq[]){
    TpReg reg;
    FILE *ptr = fopen(nomeArq, "rb");
    fseek(ptr, 0, 0);
    fread(&reg, sizeof(TpReg), 1, ptr);
    while(!feof(ptr)){
        if(reg.numero == 0)
            printf("\n");
        else
            printf("%d        %s        %.2f        %c        %d\n", reg.numero, reg.nome, reg.salario, reg.status, reg.elo);
        fread(&reg, sizeof(TpReg), 1, ptr);    
    }
    
    fclose(ptr);
}

int ultimaPos(FILE *ptr){

    fseek(ptr, 0, 2);
    return ftell(ptr)/sizeof(TpReg);
}
void reorganiza(char nomeArq[]){
    TpReg reg, regTemp;
    int i, pos, auxpos, auxhash, pont, flag, prox;
    FILE *ptr = fopen(nomeArq, "rb+");
    i = 0;
    while(i < N){
        flag = 1;

        fseek(ptr, i * sizeof(TpReg), 0);
        fread(&reg, sizeof(TpReg), 1, ptr);
        if(reg.status == 'F'){
            if(reg.elo == 0){
                reg.numero = 0;
                strcpy(reg.nome, "\0");
                reg.salario = 0;
                reg.status = 0;
                fseek(ptr, i * sizeof(TpReg), 0);
                fwrite(&reg, sizeof(TpReg), 1, ptr);

            } else{
                //Ir até quem o elo aponta
                fseek(ptr, reg.elo * sizeof(TpReg), 0);
                fread(&regTemp, sizeof(TpReg), 1, ptr);
                if(regTemp.status == 'F') // Caso tambem for falso ira requerer repetiçao
                    flag = 0;

                auxpos = reg.elo; //13 // Guarda elo para atualizar referencia da area de dados caso necessario
                //Substituir pos atual pelo reg do overflow
                fseek(ptr, i * sizeof(TpReg), 0);//0
                fwrite(&regTemp, sizeof(TpReg), 1, ptr);                
                pos = ultimaPos(ptr);//17

                //Substitui quem o elo apontava pelo ultimo reg
                if(reg.elo != pos - 1){ 
                    fseek(ptr, (pos - 1) *sizeof(TpReg), 0);
                    fread(&reg, sizeof(TpReg), 1, ptr);
                    fseek(ptr, auxpos * sizeof(TpReg), 0);
                    fwrite(&reg, sizeof(TpReg), 1, ptr);
                }

                //Apagar eof
                ftruncate(fileno(ptr), (pos - 1) * sizeof(TpReg));

                //Atualizar area de dados
                if(reg.elo != pos - 1){ //MELHORAR
                    auxhash = Hash(reg.numero);
                    fseek(ptr, auxhash * sizeof(TpReg), 0);
                    fread(&regTemp, sizeof(TpReg), 1, ptr);
                    regTemp.elo = auxpos;
                    fseek(ptr, auxhash * sizeof(TpReg), 0);
                    fwrite(&regTemp, sizeof(TpReg), 1, ptr);    
                }
            }

        } else{
            if(reg.elo != 0){
                pont = reg.elo; //16
                do{
                    //Pegar quem o elo aponta
                    fseek(ptr, pont * sizeof(TpReg), 0);
                    fread(&regTemp, sizeof(TpReg), 1, ptr);
                    //Guardar atual e prox
                    auxpos = pont;
                    prox = regTemp.elo;
                    if(regTemp.status == 'F'){
                        pos = ultimaPos(ptr);
                        //Caso o reg excluido nao for o ultimo, ir no final e pegar o substituto
                        if(pont != pos - 1){
                            fseek(ptr, (pos - 1) *sizeof(TpReg), 0);
                            fread(&reg, sizeof(TpReg), 1, ptr);
                            fseek(ptr, auxpos * sizeof(TpReg), 0);
                            fwrite(&reg, sizeof(TpReg), 1, ptr);
                        }
                        //Apagar eof
                        ftruncate(fileno(ptr), (pos - 1) * sizeof(TpReg));
                        //Atualiza area de dados
                        if(pont != pos - 1){ 
                            auxhash = Hash(reg.numero);
                            fseek(ptr, auxhash * sizeof(TpReg), 0);
                            fread(&regTemp, sizeof(TpReg), 1, ptr);
                            while(regTemp.elo != pos - 1){
                                auxhash = regTemp.elo;
                                fseek(ptr, auxhash * sizeof(TpReg), 0);
                                fread(&regTemp, sizeof(TpReg), 1, ptr);
                            }
                            regTemp.elo = auxpos;
                            fseek(ptr, auxhash * sizeof(TpReg), 0);
                            fwrite(&regTemp, sizeof(TpReg), 1, ptr);
                        }
                    }
                    pont = prox;
                }while(pont != 0);
            }
        }

        if(!flag)
            i--;
        i++;
    }
    fclose(ptr);
}
void reorganizaPrints(char nomeArq[]){
    TpReg reg, regTemp;
    int i, pos, auxpos, auxhash, pont, flag, prox;
    FILE *ptr = fopen(nomeArq, "rb+");
    i = 0;
    while(i < N){
        flag = 1;

        fseek(ptr, i * sizeof(TpReg), 0);
        fread(&reg, sizeof(TpReg), 1, ptr);

        printf("Analisando registro na posicao %d: numero=%d, status=%c, elo=%d\n", i, reg.numero, reg.status, reg.elo);

        if(reg.status == 'F'){
            if(reg.elo == 0){
                printf("Registro com status 'F' e elo 0 na posicao %d - limpando registro\n", i);
                reg.numero = 0;
                strcpy(reg.nome, "\0");
                reg.salario = 0;
                reg.status = 0;
                fseek(ptr, i * sizeof(TpReg), 0);
                fwrite(&reg, sizeof(TpReg), 1, ptr);
                printf("Registro na posicao %d apagado.\n", i);

            } else{
                //ler o apontamento
                fseek(ptr, reg.elo * sizeof(TpReg), 0);
                fread(&regTemp, sizeof(TpReg), 1, ptr);
                //decrementar para voltar nesse registro dnv
                if(regTemp.status == 'F')
                    flag = 0;
                auxpos = reg.elo;
                //coloca o registro no lugar do que quero excluir
                fseek(ptr, i * sizeof(TpReg), 0);
                fwrite(&regTemp, sizeof(TpReg), 1, ptr);
                printf("Copiado registro do elo %d para posicao %d\n", auxpos, i);
                
                pos = ultimaPos(ptr);
                if(reg.elo != pos - 1){ //MELHORAR
                    //coloca o ultimo registro no lugar da copia do outro registro
                    fseek(ptr, (pos - 1) *sizeof(TpReg), 0);
                    fread(&reg, sizeof(TpReg), 1, ptr);
                    printf("Ultimo registro: numero=%d, status=%c, elo=%d\n", reg.numero, reg.status, reg.elo);
                    fseek(ptr, auxpos * sizeof(TpReg), 0);
                    fwrite(&reg, sizeof(TpReg), 1, ptr);
                    printf("Substituido registro na posicao %d pelo ultimo registro\n", auxpos);
                }
                //gravar eof na ultima pos
                ftruncate(fileno(ptr), (pos - 1) * sizeof(TpReg));
                printf("Arquivo truncado na posicao %d\n", pos - 1);
                if(reg.elo != pos - 1){ //MELHORAR
                    //como estamos na area de overflow, temos que ver quem apontava para esse registro
                    //ir até a posição que apontava para esse overflow e alterar para a nova posicao
                    auxhash = Hash(reg.numero);
                    fseek(ptr, auxhash * sizeof(TpReg), 0);
                    fread(&regTemp, sizeof(TpReg), 1, ptr);
                    printf("Registro na posicao hash %d antes da atualizacao: elo = %d\n", auxhash, regTemp.elo);
                    regTemp.elo = auxpos;
                    fseek(ptr, auxhash * sizeof(TpReg), 0);
                    fwrite(&regTemp, sizeof(TpReg), 1, ptr);
                    printf("Registro na posicao hash %d atualizado: elo = %d\n", auxhash, regTemp.elo);
    
                }
            }

        } else{
            if(reg.elo != 0){
                pont = reg.elo;
                do{
                    //ler o apontamento
                    fseek(ptr, pont * sizeof(TpReg), 0);
                    fread(&regTemp, sizeof(TpReg), 1, ptr);
                    auxpos = pont;
                    prox = regTemp.elo;

                    if(regTemp.status == 'F'){
                        //coloca o ultimo registro no lugar da copia do outro registro
                        pos = ultimaPos(ptr);
                        if(pont != pos - 1){
                            fseek(ptr, (pos - 1) *sizeof(TpReg), 0);
                            fread(&reg, sizeof(TpReg), 1, ptr);
                            printf("Ultimo registro: numero=%d, status=%c, elo=%d\n", reg.numero, reg.status, reg.elo);
                            fseek(ptr, auxpos * sizeof(TpReg), 0);
                            fwrite(&reg, sizeof(TpReg), 1, ptr);
                            printf("Substituido registro na posicao %d pelo ultimo registro\n", auxpos);
                        }
                        //gravar eof na ultima pos
                        ftruncate(fileno(ptr), (pos - 1) * sizeof(TpReg));
                        printf("Arquivo truncado na posicao %d\n", pos - 1);
                        if(pont != pos - 1){ //MELHORAR
                            //buscar que aponta
                            auxhash = Hash(reg.numero);
                            fseek(ptr, auxhash * sizeof(TpReg), 0);
                            fread(&regTemp, sizeof(TpReg), 1, ptr);

                            while(regTemp.elo != pos - 1){
                                auxhash = regTemp.elo;
                                fseek(ptr, auxhash * sizeof(TpReg), 0);
                                fread(&regTemp, sizeof(TpReg), 1, ptr);
                            }
                            printf("Registro na posicao hash %d antes da atualizacao: elo = %d\n", auxhash, regTemp.elo);
                            regTemp.elo = auxpos;
                            fseek(ptr, auxhash * sizeof(TpReg), 0);
                            fwrite(&regTemp, sizeof(TpReg), 1, ptr);
                            printf("Registro na posicao hash %d atualizado: elo = %d\n", auxhash, regTemp.elo);
                        }
                    }
                    printf("\n");
                    pont = prox;
                }while(pont != 0);
            }
        }

        if(!flag)
            i--;
        i++;

        
        printf("\n");
    }
    fclose(ptr);
}

int main(void){
    criaArquivoHash("arquivo.dat");
    insereHash("arquivo.dat", inserirDados(1950, "Enio", 900));
    insereHash("arquivo.dat", inserirDados(1600, "Eber", 800));
    insereHash("arquivo.dat", inserirDados(3150, "Rui", 550));

    insereHash("arquivo.dat", inserirDados(2150, "Flavio", 420));
    insereHash("arquivo.dat", inserirDados(1450, "Diogo", 1200));
    insereHash("arquivo.dat", inserirDados(1100, "Antonio", 750));
    insereHash("arquivo.dat", inserirDados(1400, "Claudio", 600));
    insereHash("arquivo.dat", inserirDados(1050, "Afonso", 3200));
    insereHash("arquivo.dat", inserirDados(1000, "Ademar", 1200));

    insereHash("arquivo.dat", inserirDados(1700, "Edson", 500));
    insereHash("arquivo.dat", inserirDados(2300, "Gerson", 800));
    insereHash("arquivo.dat", inserirDados(3000, "Ivan", 630));
    insereHash("arquivo.dat", inserirDados(2600, "Luis", 5000));
    insereHash("arquivo.dat", inserirDados(2766, "Pedro", 3000));

    //
    insereHash("arquivo.dat", inserirDados(4, "TESTEELSE1", 1111));
    insereHash("arquivo.dat", inserirDados(17, "TESTEELSE2", 2222));
    insereHash("arquivo.dat", inserirDados(30, "TESTEELSE3", 3333));
    //
    insereHash("arquivo.dat", inserirDados(10, "TESTEIF4", 4444));
    insereHash("arquivo.dat", inserirDados(23, "TESTEIF5", 5555));
    insereHash("arquivo.dat", inserirDados(36, "TESTEIF6", 6666));


    excluir("arquivo.dat", 1950);
    excluir("arquivo.dat", 1600);
    excluir("arquivo.dat", 2150);
    excluir("arquivo.dat", 1050);

    excluir("arquivo.dat", 3000);
    excluir("arquivo.dat", 2766);
    excluir("arquivo.dat", 2300);

    //AJUSTAR NO 
    excluir("arquivo.dat", 10);
    excluir("arquivo.dat", 23);
    excluir("arquivo.dat", 36);
    
    //AJUSTAR NO ELSE
    excluir("arquivo.dat", 4);
    excluir("arquivo.dat", 17);
    excluir("arquivo.dat", 30);

    exibir("arquivo.dat");
    

    printf("\n\n\n\n\n");
    //system("cls");

    reorganiza("arquivo.dat");
    exibir("arquivo.dat");
    system("pause");
    return 0;
}
