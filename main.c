#include "funcoes.h"

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
  // Inicialização da matriz de distâncias euclidianas e cálculo delas entre as
  // flores
  double **distEuc = malloc(150 * sizeof(double *));
  for (int i = 0; i < 150; i++)
  {
    distEuc[i] = malloc(150 * sizeof(double));
  }
  int iMaxEuc = 0, jMaxEuc = 0, iMinEuc = 0, jMinEuc = 0;
  montagemDistanciaEuclidiana(distEuc, flores, &iMaxEuc, &jMaxEuc, &iMinEuc,
                              &jMinEuc);

  // Teste distancias euclidianas em txt
  FILE *teste = fopen("distEuclidianas.txt", "wt");
  if (testesDistanciaEuclidiana(teste, distEuc) == 0)
  {
    printf("Erro na abertura do arquivo!\n");
    return 1;
  }

  // Inicialização da matriz de distâncias euclidianas normalizadas e cálculo
  // delas entre as flores
  double **distanciasEuclidianasNormalizadas;
  distanciasEuclidianasNormalizadas = malloc(150 * sizeof(double *));
  for (int i = 0; i < 150; i++)
  {
    distanciasEuclidianasNormalizadas[i] = malloc(150 * sizeof(double));
  }
  int iMaxNorm = 0, jMaxNorm = 0, iMinNorm = 0, jMinNorm = 0;
  montagemNormalizacaoDistanciasEuclidianas(
      distanciasEuclidianasNormalizadas, distEuc, &iMaxNorm, &jMaxNorm,
      &iMinNorm, &jMinNorm, distEuc[iMinEuc][jMinEuc],
      distEuc[iMaxEuc][jMaxEuc]);

  // Teste distancias normalizadas
  FILE *teste2 = fopen("distEuclidianasNormalizadas.txt", "wt");
  if (testesNormalizacao(teste2, distanciasEuclidianasNormalizadas) == 0)
  {
    printf("Erro na abertura do arquivo!\n");
    return 1;
  }

  // Inicialização da matriz adjacência e ligação dos nodos caso a distância
  // euclidiana normalizada seja <= 0,2
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

  // Realizacao da dfs para verificar se existe conexao entre flores
  int *vetContido;
  vetContido = calloc(150, sizeof(int));
  dfs(0, matrizAdj, vetContido);


  int *vetContido2;
  vetContido2 = calloc(150, sizeof(int));
  dfs(50, matrizAdj, vetContido2);


  // Escrita do csv de resposta da Tarefa_1A
  FILE *data = fopen("data.csv", "wt");
  if (escritaResposta(data, matrizAdj,
                      distanciasEuclidianasNormalizadas[iMaxNorm][jMaxNorm],
                      distanciasEuclidianasNormalizadas[iMinNorm][jMinNorm],
                      iMaxNorm, jMaxNorm, iMinNorm, jMinNorm,
                      distEuc[iMaxEuc][jMaxEuc], distEuc[iMinEuc][jMinEuc],
                      iMaxEuc, jMaxEuc, iMinEuc, jMinEuc) == 0)
  {
    printf("Erro na abertura do arquivo!\n");
    return 1;
  }

  // Flor *setosas, *nSetosas, *floresPredicoes;
  // setosas = malloc(150*sizeof(Flor));
  // nSetosas = malloc(150*sizeof(Flor));
  // floresPredicoes = malloc(150*sizeof(Flor));
  // setosas[0] = flores[0];
  // nSetosas[0] = flores[50];
  // Flor mSet = media(setosas, vetContido);
  // Flor mNSet = media(nSetosas,vetContido2);
  // int h = 1, k = 1;
  // char auxSet[11] = "Setosa", auxN[11] = "NaoSetosa";
  // for (int i = 0; i < 150; i++)
  // {
  //     floresPredicoes[i].pl = flores[i].pl;
  //     floresPredicoes[i].pw = flores[i].pw;
  //     floresPredicoes[i].sl = flores[i].sl;
  //     floresPredicoes[i].sw = flores[i].sw;
  //   if (verificaPelaMedia(flores[i], mSet, mNSet) == 1)
  //   {
  //     setosas[h] = flores[i];
  //     strcpy(floresPredicoes[i].tipo, auxSet);
  //     vetContido[i] = 1;
  //     h++;
  //   }else
  //   {
  //     strcpy(floresPredicoes[i].tipo, auxN);
  //     nSetosas[k] = flores[i];
  //     vetContido2[i] = 1;
  //     k++;
  //   }
  //   mSet = media(setosas,vetContido);
  //   mNSet = media(nSetosas, vetContido2);
  //   printf("%s\n", floresPredicoes[i].tipo);
  // }
  
  // printf("%.2lf\n", comparaPredicao(floresPredicoes, vetContido));


  FILE *teste4 = fopen("marcasRaiz50.csv", "wt");
  if (teste4 != NULL)
  {
    for (int i = 1; i < 150; i++)
    {
      fprintf(teste4, "%d\n", vetContido2[i]);
    }
  }

  FILE *testes3 = fopen("marcasRaiz0.csv", "wt");
  if (testes3 != NULL)
  {
    for (int i = 1; i < 150; i++)
    {
      fprintf(testes3, "%d\n", vetContido[i]);
    }
  }

  // Liberacao de memoria
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
  // free(floresPredicoes);
  return 0;
}

