#include "intercalacao.h"
#include "funcionarios.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct no
{
    int info;
    struct no *esq;
    struct no *dir;
} TNo;

void intercalacao_basico(char *nome_arquivo_saida, int num_p, Lista *nome_particoes)
{

    int fim = 0; //variavel que controla fim do procedimento
    FILE *out;   //declara ponteiro para arquivo

    //abre arquivo de saida para escrita
    if ((out = fopen(nome_arquivo_saida, "wb")) == NULL)
    {
        printf("Erro ao abrir arquivo de sa?da\n");
    }
    else
    {
        //cria vetor de particoes
        TVet v[num_p];

        //abre arquivos das particoes, colocando variavel de arquivo no campo f do vetor
        //e primeiro funcionario do arquivo no campo func do vetor
        for (int i = 0; i < num_p; i++)
        {
            v[i].f = fopen(nome_particoes->nome, "rb");
            v[i].aux_p = 0;
            if (v[i].f != NULL)
            {
                fseek(v[i].f, v[i].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[i].f);
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posi??o do vetor
                    v[i].func = funcionario(INT_MAX, "", "", "", 0);
                }
                else
                {
                    //conseguiu ler funcionario, coloca na posi??o atual do vetor
                    v[i].func = f;
                }
            }
            else
            {
                fim = 1;
            }
            nome_particoes = nome_particoes->prox;
        }

        int aux = 0;
        while (!(fim))
        { //conseguiu abrir todos os arquivos
            int menor = INT_MAX;
            int pos_menor;
            //encontra o funcionario com menor chave no vetor
            for (int i = 0; i < num_p; i++)
            {
                if (v[i].func->cod < menor)
                {
                    menor = v[i].func->cod;
                    pos_menor = i;
                }
            }
            if (menor == INT_MAX)
            {
                fim = 1; //terminou processamento
            }
            else
            {
                //salva funcionario no arquivo de saída
                fseek(out, aux * tamanho_registro(), SEEK_SET);
                salva_funcionario(v[pos_menor].func, out);
                //atualiza posição pos_menor do vetor com pr?ximo funcionario do arquivo
                v[pos_menor].aux_p++;
                fseek(v[pos_menor].f, v[pos_menor].aux_p * tamanho_registro(), SEEK_SET);
                TFunc *f = le_funcionario(v[pos_menor].f);
                aux++;
                if (f == NULL)
                {
                    //arquivo estava vazio
                    //coloca HIGH VALUE nessa posiçao do vetor
                    v[pos_menor].func = funcionario(INT_MAX, "", "", "", 0.0);
                }
                else
                {
                    v[pos_menor].func = f;
                }
            }
        }

        //fecha arquivos das partiÇões de entrada
        for (int i = 0; i < num_p; i++)
        {
            fclose(v[i].f);
            //    free(v[i].func);
        }
        //fecha arquivo de saída
        fclose(out);
    }
}

void removePrimeiroItem(TNo *vetNo[], int num_p)
{
    for (int i = 0; i < num_p; i++)
    {
        if (i + 1 < num_p && vetNo[i + 1] != NULL)
        {
            vetNo[i] = vetNo[i + 1];
        }
        else
        {
            vetNo[i] = NULL;
        }
    }
}

void adicionaNaLista(TNo *vetNo[], TNo *tempNo, int num_p)
{
    for (int i = 0; i < num_p; i++)
    {
        if (vetNo[i] == NULL)
        {
            vetNo[i] = tempNo;
        }
    }
}

void gravaFuncArquivoSaida(FILE *arquivoSaida, TPilha **pilha, TNo *vetNo[], TNo *tempNo, int num_p)
{
    for (int i = 0; i < num_p; i++)
    {
        if (vetNo[0]->info == pilha[i]->info->cod)
        {
            //Gravar funcionario no arquivo
            salva_ArqSaida(arquivoSaida, vetNo[0]->info, pilha[i]);

            //TODO: Corrigir - método de remoção errado, usar o pop
            free(pilha[i]); // LIBERANDO O ITEM GRAVADO DA PILHA
        }
    }
}

void intercalacao_arvore_de_vencedores(TPilha **pilha, int *vetTop, char *nome_arquivo_saida, int num_p)
{

    TNo *vetNo[num_p], *tempNo;
    int flag = -1;
    FILE *arquivoSaida = fopen(nome_arquivo_saida, "a+b");

    do
    {
        for (int i = 0; i < num_p; i++)
        {
            if (pilha[i] != NULL)
            {
                TNo *n = (TNo *)malloc(sizeof(TNo));

                n->info = pilha[i]->info->cod;

                vetNo[i] = n;
            }
            else
            {
                vetNo[i] = NULL;
            }
        }

        if (vetNo[1] == NULL)
        {
            gravaFuncArquivoSaida(arquivoSaida, pilha, vetNo, tempNo, num_p);
            break;
        }

        do
        {
            for (int i = 0; i < num_p; i++)
            {
                printf("vetNo[%d] =======> %d \n", i, vetNo[i]->info);
            }
            TNo *aux1, *aux2;
            TNo *n = (TNo *)malloc(sizeof(TNo));
            aux1 = vetNo[0];
            removePrimeiroItem(vetNo, num_p);

            aux2 = vetNo[0];
            removePrimeiroItem(vetNo, num_p);

            tempNo = NULL;

            for (int i = 0; i < num_p; i++)
            {
                printf("vetNo[%d] =======> %d \n", i, vetNo[i]->info);
            }

            printf("A =======> %d \n", aux1->info);
            printf("B =======> %d\n", aux2->info);
            int teste = aux1->info > aux2->info ? aux2->info : aux1->info;

            n->info = teste;
            n->esq = aux1;
            n->dir = aux2;

            adicionaNaLista(vetNo, n, num_p);

        } while (vetNo[1] != NULL);

        gravaFuncArquivoSaida(arquivoSaida, pilha, vetNo, tempNo, num_p);

    } while (flag == -1);

    /*

    Criar uma lista de nós vazia;


    do{
        zera a lista;

        Criar nós folha da árvore, e adicioná-los na lista

        Pegar o primeiro elemento de cada pilha e inserir na raiz de cada nó;

        Se a lista de nós possuir somente um elemento
        {
            Coloca o elemento no arquivo de saída
            break;
        }

        do{

            1) Retirar os 2 primeiros nós da lista
            2) Criar um nó p para ser o pai desses dois, escolhendo o vencedor e ajustando os campos do nó
            criado de acordo
            3) Adicionar o nó p no final da lista
            --O elemento que sobrou na lista é a raiz da árvore de vencedores

        }while(Enquanto lista tiver mais de 1 elemento)

        Pegar a raiz da primeira posição

        Alocar no arquivo de saída

        desempilhar o elemento equivalente ao raiz, na pilha;

    }while(flag == -1)

    */
}
