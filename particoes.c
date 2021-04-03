
#include "funcionarios.h"
#include <stdlib.h>
#include <limits.h>

#include "particoes.h"

void classificacao_interna(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc) {
    rewind(arq); //posiciona cursor no inicio do arquivo

    int reg = 0;

    while (reg != nFunc) {
        //le o arquivo e coloca no vetor
        TFunc *v[M];
        int i = 0;
        while (!feof(arq)) {
            fseek(arq, (reg) * tamanho_registro(), SEEK_SET);
            v[i] = le_funcionario(arq);
            imprime_funcionario(v[i]);
       //     imprime_funcionario(v[i]);
            i++;
            reg++;
            if(i>=M) break;
        }
        printf("%d - %d \n", i, M);
        //ajusta tamanho M caso arquivo de entrada tenha terminado antes do vetor
        /*if (i != M) {
            M = i;
            printf("entrou - 4 \n");
        }*/
        printf("%d - %d \n", i, M);
        //faz ordenacao
        for (int j = 1; j < M; j++) {
                printf("entrou - 5 \n");
            TFunc *f = v[j];
            i = j - 1;
            while ((i >= 0) && (v[i]->cod > f->cod)) {
                    printf("entrou while - 5 \n");
                v[i + 1] = v[i];
                i = i - 1;
            }
            v[i + 1] = f;
        }

        //cria arquivo de particao e faz gravacao
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;
        printf("\n%s\n", nome_particao);
        FILE *p;
        printf("entrou - 6 \n");
        if ((p = fopen(nome_particao, "wb+")) == NULL) {
            printf("Erro criar arquivo de saida\n");
        } else {
            for (int i = 0; i < M; i++) {
                fseek(p, (i) * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[i], p);
                imprime_funcionario(v[i]);
                printf("entrou - 7 \n");
            }
            fclose(p);
        }
        for(int jj = 0; jj<M; jj++)
            free(v[jj]);
    }
}

int menorChave(TFunc *array[], int size){
    int posMenor, i, posNotNULL;
    char posNull = 'N';
    TFunc *menor;

    for(i = 0; i < size; i++){
        if(array[i] == NULL){
            printf("---%d--- posicao null \n", i);
            posNull = 'S';
        }else{
            posNotNULL = i;
        }
    }
    if(posNull == 'N'){
        for(i = 0; i < size; i++){
            if(i == 0){
                menor = array[0];
                posMenor = 0;
            }else if (menor->cod > array[i]->cod){
                menor = array[i];
                posMenor = i;
            }
        }
    }else{
        return posNotNULL;
        }

    return posMenor;
}

void salva_ArqSaida(FILE *saida, int indice, TFunc *item){
        fseek(saida, (indice)*tamanho_registro(), SEEK_SET);
        salva_funcionario(item, saida);
}

void selecao_natural(FILE *arq, Lista *nome_arquivos_saida, int M, int nFunc, int n){

    rewind(arq);
    TFunc *array[M], *reservatorio[M], *menorRegistro, *proxRegistroEntrada;
    int count = 0;
    int posArqEntrada = 0;
    int posMenorArray = 0;
    int posArqSaida = 0;
    int posReservatorio = 0;

    /* LER M REGISTROS DO ARQUIVO PARA A MEM�RIA */
    if(arq != NULL){
        while(count < M){
            fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
            array[count] = le_funcionario(arq);
            count ++;
            posArqEntrada ++;
            /* CERTO */
        }

        /* GRAVAR O REGISTRO DE MENOR CHAVE NA PARTI��O DE SA�DA */
        char *nome_particao = nome_arquivos_saida->nome;
        nome_arquivos_saida = nome_arquivos_saida->prox;

        FILE *arquivoSaida;
        arquivoSaida = fopen(nome_particao, "wb+");

        /* CASO AINDA EXISTA ESPA�O LIVRE NO RESERVAT�RIO */
        do{
            /* SELECIONAR NO ARRAY EM MEM�RIA, O REGISTRO R COM MENOR CHAVE */
            posMenorArray = menorChave(array, M);
            menorRegistro = array[posMenorArray];

            /* CERTO */
            if (arquivoSaida != NULL){
                salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                posArqSaida++;
            }
            /* CERTO */

            /* SUBSTITUIR NO ARRAY EM MEM�RIA O MENOR REGISTRO PELO PR�XIMO DO ARQUIVO DE ENTRADA */
            fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
            proxRegistroEntrada = le_funcionario(arq);
            array[posMenorArray] = proxRegistroEntrada;
            posArqEntrada ++;
            /* CERTO */

            /* CASO A CHAVE DO PROXIMO REGISTRO DE ENTRADA SEJA MENOR DO QUE O MENOR REGISTRO
             * GRAVA NO RESERVAT�RIO E SUBSTITUI NO ARRAY DE MEM�RIA O MENOR REGISTRO PELO PROXIMO REGISTRO DE ENTRADA
             */
            if(feof(arq)){
                posMenorArray = menorChave(array, M);
                menorRegistro = array[posMenorArray];
                salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                break;
            }
            if(proxRegistroEntrada->cod < menorRegistro->cod){
                /* GRAVA NO RESERVAT�RIO */
                if(posArqEntrada < nFunc){

                    reservatorio[posReservatorio] = proxRegistroEntrada;
                    posReservatorio++;

                    /* SUBSTITUIR NO ARRAY EM MEM�RIA O MENOR REGISTRO PELO PROXIMO REGISTRO DO ARQUIVO DE ENTRADA */
                    posArqEntrada++;
                    fseek(arq, (posArqEntrada)*tamanho_registro(), SEEK_SET);
                    proxRegistroEntrada = le_funcionario(arq);
                    array[posMenorArray] = proxRegistroEntrada;
                   // array[posMenorArray] = proxRegistroEntrada;
                }
                else{
                    reservatorio[posReservatorio] = proxRegistroEntrada;
                    array[posMenorArray] = NULL;

                    posMenorArray = menorChave(array, M);
                    menorRegistro = array[posMenorArray];
                    if (arquivoSaida != NULL){
                        salva_ArqSaida(arquivoSaida, posArqSaida, menorRegistro);
                        posArqSaida++;
                    }

                    posReservatorio++;
                }
            }

            if(posReservatorio < M){
                /* VERIFICA QUANTIDADE DE ITENS NO RESERVAT�RIO */
            }else{
                /* FECHAR A PARTI��O DE SA�DA */
                fclose(arquivoSaida);

                /* COPIAR OS REGISTROS DO RESERVAT�RIO PARA O ARRAY EM MEM�RIA */
                for(int i = 0; i < posReservatorio; i ++){
                    array[i] = reservatorio[i];
                }

                /* ESVAZIAR O RESERVAT�RIO */
                for(int r = 0; r < M; r++){
                    reservatorio[r] = NULL;
                }
                posReservatorio = 0;

                /* ABRIR NOVO ARQUIVO DE SA�DA */
                char *nome_particao = nome_arquivos_saida->nome;
                nome_arquivos_saida = nome_arquivos_saida->prox;

                FILE *arquivoSaida;
                arquivoSaida = fopen(nome_particao, "wb+");
                posArqSaida = 0;
            }
        }while(posReservatorio < M);
    }
}
