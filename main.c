#include <stdio.h>
#include <stdlib.h>
#include "distancias.h"

int leituraIrisDataset(Flor *flores, FILE *arquivoCSV)
{
    if (arquivoCSV != NULL)
    {
        char lixo[100];
        fscanf(arquivoCSV, "%[^\n]", lixo);
        for (int i = 0; i < 150; i++)
        {
            for (int j = 0; j < 11; j++)
            {
                flores[i].tipo[j] = '\0';
            }
            fscanf(arquivoCSV, "\n%[^,],", flores[i].tipo); // Lê até 10 caracteres ou até a vírgula
            fscanf(arquivoCSV, "%lf,%lf,%lf,%lf", &flores[i].sl, &flores[i].sw, &flores[i].pl, &flores[i].pw);
        }
        fclose(arquivoCSV);
        return 1;
    }
    else
    {
        return 0;
    }
}

void calculoDistanciaEuclidiana(double **distEuc, Flor *flores, int *linhaMaximoEuclidiano, 
int *colunaMaximoEuclidiano, int *linhaMinimoEuclidiano, int *colunaMinimoEuclidiano)
{
    double aux = 0;
    int iMaxEuc = 0, jMaxEuc = 0, iMinEuc = 0, jMinEuc = 0;
    for (int i = 0; i < 150; i++)
    {
        distEuc[i][i] = 0;
        for (int j = i + 1; j < 150; j++)
        {
            aux = distEucl(&flores[i], &flores[j]);
            distEuc[i][j] = aux;
            distEuc[j][i] = aux;
            // Pega a posição do máximo euclidiano encontrado e do mínimo, caso seja a primeira iteração
            if (aux > distEuc[iMaxEuc][jMaxEuc])
            {
                iMaxEuc = i;
                jMaxEuc = j;
                if (i == 0)
                {
                    iMinEuc = i;
                    jMinEuc = j;
                }
            }
            // Pega a posição do minimo euclidiano
            if (aux < distEuc[iMinEuc][jMinEuc])
            {
                iMinEuc = i;
                jMinEuc = j;
            }
        }
    }
    (*linhaMaximoEuclidiano) = iMaxEuc;
    (*colunaMaximoEuclidiano) = jMaxEuc;
    (*linhaMinimoEuclidiano) = iMinEuc;
    (*colunaMinimoEuclidiano) = jMinEuc;
}

int testesDistanciaEuclidiana(FILE *csvDestino, double **distanciasEuclidianas)
{
    if (csvDestino != NULL)
    {
        for (int i = 0; i < 150; i++)
        {
            for (int j = i + 1; j < 150; j++)
            {

                fprintf(csvDestino, "No 1: %d, No2: %d, DE: %.2lf\n", i, j, distanciasEuclidianas[i][j]);
            }
        }
        fclose(csvDestino);
        return 1;
    }
    else
    {
        return 0;
    }
}

void normalizacaoDistanciasEuclidianas(double **distanciasNormalizadas, double **distanciasEuclidianas, 
int *linhaMaximoNormalizado, int *colunaMaximoNormalizado, int *linhaMinimoNormalizado, int *colunaMinimoNormalizado, 
double minimoEuclidiano, double maximoEuclidiano)
{
    double aux = 0;
    int iMaxNorm = 0, jMaxNorm = 0, iMinNorm = 0, jMinNorm = 0;
    for (int i = 0; i < 150; i++)
    {
        distanciasNormalizadas[i][i] = 0;
        for (int j = i + 1; j < 150; j++)
        {
            aux = distEuclNorm(distanciasEuclidianas, i, j, minimoEuclidiano, maximoEuclidiano);
            distanciasNormalizadas[i][j] = aux;
            distanciasNormalizadas[j][i] = aux;
            // Pega a posição do maior euclidiano normalizado e do menor, caso seja a primeira iteração
            if (aux > distanciasNormalizadas[iMaxNorm][jMaxNorm])
            {
                iMaxNorm = i;
                jMaxNorm = j;
                if (i == 0)
                {
                    iMinNorm = i;
                    jMinNorm = j;
                }
            }
            // Pega a posição do mínimo euclidiano normalizado
            if (aux < distanciasNormalizadas[iMinNorm][jMinNorm])
            {
                iMinNorm = i;
                jMinNorm = j;
            }
        }
    }
    (*linhaMaximoNormalizado) = iMaxNorm;
    (*colunaMaximoNormalizado) = jMaxNorm;
    (*linhaMinimoNormalizado) = iMinNorm;
    (*colunaMinimoNormalizado) = jMinNorm;
}

void montarMatrizAdjacencia(int **matrizAdjacencia, double **distanciasNormalizadas)
{
    for (int i = 0; i < 150; i++)
    {
        matrizAdjacencia[i][i] = 0;
        for (int j = i + 1; j < 150; j++)
        {
            if (distanciasNormalizadas[i][j] <= 0.2)
            {
                matrizAdjacencia[i][j] = 1;
                matrizAdjacencia[j][i] = 1;
            }
            else
            {
                matrizAdjacencia[i][j] = 0;
                matrizAdjacencia[j][i] = 0;
            }
        }
    }
}

int testesNormalizacao(FILE *csvDestino, double **distanciasNormalizadas)
{
    if (csvDestino != NULL)
    {
        for (int i = 0; i < 150; i++)
        {
            for (int j = i + 1; j < 150; j++)
            {

                fprintf(csvDestino, "No 1: %d, No2: %d, DE: %.2lf\n", i, j, distanciasNormalizadas[i][j]);
            }
        }
        fclose(csvDestino);
        return 1;
    }
    else
    {
        return 0;
    }
}

int escritaResposta(FILE *csvDestino, int **matrizAdjacencia, double maxDistanciasNormalizadas, double minDistanciasNormalizadas, int iMaxNorm, 
int jMaxNorm, int iMinNorm, int jMinNorm, double maxDistanciasEuclidianas, double minDistanciasEuclidianas, int iMaxEuc, int jMaxEuc, 
int iMinEuc, int jMinEuc)
{
    if (csvDestino != NULL)
    {
        fprintf(csvDestino, "150,%.2lf,%i,%i,%.2lf,%i,%i,%.2lf,%i,%i,%.2lf,%i,%i\n", maxDistanciasEuclidianas, iMaxEuc, jMaxEuc,
                minDistanciasEuclidianas, iMinEuc, jMinEuc, maxDistanciasNormalizadas, iMaxNorm, jMaxNorm, minDistanciasNormalizadas,
                iMinNorm, jMinNorm);
        for (int i = 0; i < 150; i++)
        {
            for (int j = i + 1; j < 150; j++)
            {
                if (matrizAdjacencia[i][j] == 1)
                    fprintf(csvDestino, "%i,%i\n", i, j);
            }
        }
        fclose(csvDestino);
        return 1;
    }
    else
    {
        return 0;
    }
}

int testesMatrizAdjacencia(FILE *txtDestino, int **matrizAdjacencia)
{
    if (txtDestino != NULL)
    {
        for (int i = 0; i < 150; i++)
        {
            for (int j = 0; j < 150; j++)
            {
                fprintf(txtDestino, "Conexao %d a %d: %d\n", i, j, matrizAdjacencia[i][j]);
            }
        }
        fclose(txtDestino);
        return 1;
    }
    else
    {
        return 0;
    }
}

int main()
{

    Flor *flores;
    flores = malloc(150 * sizeof(Flor));
    // Abertura e coleta dos dados do arquivo csv
    FILE *f = fopen("IrisDataset.csv", "rt");
    if (leituraIrisDataset(flores, f) == 0)
    {
        printf("Erro na abertura de arquivo\n");
        return 1;
    }

    // Inicialização da matriz de distâncias euclidianas e cálculo delas entre as flores
    double **distEuc = malloc(150 * sizeof(double *));
    for (int i = 0; i < 150; i++)
    {
        distEuc[i] = malloc(150 * sizeof(double));
    }
    int iMaxEuc = 0, jMaxEuc = 0, iMinEuc = 0, jMinEuc = 0;
    calculoDistanciaEuclidiana(distEuc, flores, &iMaxEuc, &jMaxEuc, &iMinEuc, &jMinEuc);

    // Teste distancias euclidianas em csv
    FILE *teste = fopen("distEuclidianas.csv", "wt");
    if (testesDistanciaEuclidiana(teste, distEuc) == 0)
    {
        printf("Erro na abertura do arquivo!\n");
        return 1;
    }

    // Inicialização da matriz de distâncias euclidianas normalizadas e cálculo delas entre as flores
    double **distanciasEuclidianasNormalizadas;
    distanciasEuclidianasNormalizadas = malloc(150 * sizeof(double *));
    for (int i = 0; i < 150; i++)
    {
        distanciasEuclidianasNormalizadas[i] = malloc(150 * sizeof(double));
    }
    int iMaxNorm = 0, jMaxNorm = 0, iMinNorm = 0, jMinNorm = 0;
    normalizacaoDistanciasEuclidianas(distanciasEuclidianasNormalizadas, distEuc, &iMaxNorm, &jMaxNorm, &iMinNorm, &jMinNorm, 
    distEuc[iMinEuc][jMinEuc], distEuc[iMaxEuc][jMaxEuc]);

    // Teste distancias normalizadas
    FILE *teste2 = fopen("distEuclidianasNormalizadas.csv", "wt");
    if (testesNormalizacao(teste2, distanciasEuclidianasNormalizadas) == 0)
    {
        printf("Erro na abertura do arquivo!\n");
        return 1;
    }

    // Inicialização da matriz adjacência e ligação dos nodos caso a distância euclidiana normalizada seja <= 0,2
    int **matrizAdj;
    matrizAdj = malloc(150 * sizeof(int *));
    for (int i = 0; i < 150; i++)
    {
        matrizAdj[i] = calloc(150, sizeof(int));
    }
    montarMatrizAdjacencia(matrizAdj, distanciasEuclidianasNormalizadas);

    // Teste matriz adjacencia
    FILE *mAdj = fopen("matrizAdj.txt", "wt");
    if (testesMatrizAdjacencia(mAdj, matrizAdj) == 0)
    {
        printf("Erro na abertura do arquivo!\n");
        return 1;
    }

    // int *vetContido;
    // vetContido = calloc(150,sizeof(int));
    // dfs(0, matrizAdj,vetContido);
    // FILE *testes3 = fopen("marcasRaiz0.csv", "wt");
    // if (testes3 != NULL)
    // {
    //     for (int i = 1; i < 150; i++)
    //     {
    //         fprintf(testes3,"%d\n", vetContido[i]);
    //     }

    // }

    //    int *vetContido2;
    // vetContido2 = calloc(150,sizeof(int));
    // dfs(50,matrizAdj,vetContido2);
    // FILE *teste4 = fopen("marcasRaiz50.csv", "wt");
    // if (teste4 != NULL)
    // {
    //     for (int i = 1; i < 150; i++)
    //     {
    //         fprintf(teste4, "%d\n", vetContido2[i]);
    //     }

    // }

    // Escrita do csv de resposta da Tarefa_1A
    FILE *data = fopen("data.csv", "wt");
    if (escritaResposta(data, matrizAdj, distanciasEuclidianasNormalizadas[iMaxNorm][jMaxNorm], 
    distanciasEuclidianasNormalizadas[iMinNorm][jMinNorm], 
    iMaxNorm, jMaxNorm, iMinNorm, jMinNorm, distEuc[iMaxEuc][jMaxEuc], 
    distEuc[iMinEuc][jMinEuc], iMaxEuc, jMaxEuc, iMinEuc, jMinEuc) == 0)
    {
        printf("Erro na abertura do arquivo!\n");
        return 1;
    }


    //Liberacao de memoria
    for (int i = 0; i < 150; i++)
    {
        free(distEuc[i]);
        free(distanciasEuclidianasNormalizadas[i]);
        free(matrizAdj[i]);
    }
    free(matrizAdj);
    free(distEuc);
    free(distanciasEuclidianasNormalizadas);
    free(flores);
    return 0;
}
