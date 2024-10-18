#include "funcoes.h"

// Calculo da distanca euclidiana
double distEucl(Flor *f1, Flor *f2) {
  return sqrt(pow((f2->sl - f1->sl), 2) + pow((f2->sw - f1->sw), 2) +
              pow((f2->pl - f1->pl), 2) + pow((f2->pw - f1->pw), 2));
}

// Calculo da distancia euclidiana normalizada
double distEuclNorm(double **distEuc, int linha, int coluna, double min,
                    double max) {
  return (distEuc[linha][coluna] - min) / (max - min);
}

// DFS
void dfs(int raiz, int **matAdj, int *vetContido) {
  vetContido[raiz] = 1;
  for (int i = 0; i < 150; i++) {
    if (matAdj[raiz][i] == 1) {
      if (vetContido[i] == 0) {
        dfs(i, matAdj, vetContido);
      }
    }
  }
}

// Leitura do IrisDataset
int leituraIrisDataset(Flor *flores, FILE *arquivoCSV) {
  if (arquivoCSV != NULL) {
    char lixo[100];
    fscanf(arquivoCSV, "%[^\n]", lixo);
    for (int i = 0; i < 150; i++) {
      for (int j = 0; j < 11; j++) {
        flores[i].tipo[j] = '\0';
      }
      fscanf(arquivoCSV, "\n%[^,],",
             flores[i].tipo); // Lê até 10 caracteres ou até a vírgula
      fscanf(arquivoCSV, "%lf,%lf,%lf,%lf", &flores[i].sl, &flores[i].sw,
             &flores[i].pl, &flores[i].pw);
    }
    fclose(arquivoCSV);
    return 1;
  } else {
    return 0;
  }
}

// Montagem da matriz de distancias euclidianas (relação 1 para todos)
void montagemDistanciaEuclidiana(double **distEuc, Flor *flores,
                                 int *linhaMaximoEuclidiano,
                                 int *colunaMaximoEuclidiano,
                                 int *linhaMinimoEuclidiano,
                                 int *colunaMinimoEuclidiano) {
  double aux = 0;
  int iMaxEuc = 0, jMaxEuc = 0, iMinEuc = 0, jMinEuc = 0;
  for (int i = 0; i < 150; i++) {
    distEuc[i][i] = 0;
    for (int j = i + 1; j < 150; j++) {
      aux = distEucl(&flores[i], &flores[j]);
      distEuc[i][j] = aux;
      distEuc[j][i] = aux;
      // Pega a posição do máximo euclidiano encontrado e do mínimo, caso seja a
      // primeira iteração
      if (aux > distEuc[iMaxEuc][jMaxEuc]) {
        iMaxEuc = i;
        jMaxEuc = j;
        if (i == 0) {
          iMinEuc = i;
          jMinEuc = j;
        }
      }
      // Pega a posição do minimo euclidiano
      if (aux < distEuc[iMinEuc][jMinEuc]) {
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

// Persistencia das distancias euclidianas em um txt
int testesDistanciaEuclidiana(FILE *txtDestino,
                              double **distanciasEuclidianas) {
  if (txtDestino != NULL) {
    for (int i = 0; i < 150; i++) {
      for (int j = i + 1; j < 150; j++) {

        fprintf(txtDestino, "No 1: %d, No2: %d, DE: %.2lf\n", i, j,
                distanciasEuclidianas[i][j]);
      }
    }
    fclose(txtDestino);
    return 1;
  } else {
    return 0;
  }
}

// Monagem da matriz euclidiana com valores normalizados (relacao eh a mesma da
// montagem da matriz euclidiana - 1 para todos)
void montagemNormalizacaoDistanciasEuclidianas(
    double **distanciasNormalizadas, double **distanciasEuclidianas,
    int *linhaMaximoNormalizado, int *colunaMaximoNormalizado,
    int *linhaMinimoNormalizado, int *colunaMinimoNormalizado,
    double minimoEuclidiano, double maximoEuclidiano) {
  double aux = 0;
  int iMaxNorm = 0, jMaxNorm = 0, iMinNorm = 0, jMinNorm = 0;
  for (int i = 0; i < 150; i++) {
    distanciasNormalizadas[i][i] = 0;
    for (int j = i + 1; j < 150; j++) {
      aux = distEuclNorm(distanciasEuclidianas, i, j, minimoEuclidiano,
                         maximoEuclidiano);
      distanciasNormalizadas[i][j] = aux;
      distanciasNormalizadas[j][i] = aux;
      // Pega a posição do maior euclidiano normalizado e do menor, caso seja a
      // primeira iteração
      if (aux > distanciasNormalizadas[iMaxNorm][jMaxNorm]) {
        iMaxNorm = i;
        jMaxNorm = j;
        if (i == 0) {
          iMinNorm = i;
          jMinNorm = j;
        }
      }
      // Pega a posição do mínimo euclidiano normalizado
      if (aux < distanciasNormalizadas[iMinNorm][jMinNorm]) {
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

// Persistencia dos valores das distancias euclidianas normalizadas em um txt
int testesNormalizacao(FILE *txtDestino, double **distanciasNormalizadas) {
  if (txtDestino != NULL) {
    for (int i = 0; i < 150; i++) {
      for (int j = i + 1; j < 150; j++) {

        fprintf(txtDestino, "No 1: %d, No2: %d, DE: %.2lf\n", i, j,
                distanciasNormalizadas[i][j]);
      }
    }
    fclose(txtDestino);
    return 1;
  } else {
    return 0;
  }
}

// Montagem da matriz de adjacencia
void montarMatrizAdjacencia(int **matrizAdjacencia,
                            double **distanciasNormalizadas) {
  for (int i = 0; i < 150; i++) {
    matrizAdjacencia[i][i] = 0;
    for (int j = i + 1; j < 150; j++) {
      if (distanciasNormalizadas[i][j] <= 0.2) {
        matrizAdjacencia[i][j] = 1;
        matrizAdjacencia[j][i] = 1;
      } else {
        matrizAdjacencia[i][j] = 0;
        matrizAdjacencia[j][i] = 0;
      }
    }
  }
}

// Persistencia dos valores da matriz de adjacencia em um txt
int testesMatrizAdjacencia(FILE *txtDestino, int **matrizAdjacencia) {
  if (txtDestino != NULL) {
    for (int i = 0; i < 150; i++) {
      for (int j = 0; j < 150; j++) {
        fprintf(txtDestino, "Conexao %d a %d: %d\n", i, j,
                matrizAdjacencia[i][j]);
      }
    }
    fclose(txtDestino);
    return 1;
  } else {
    return 0;
  }
}

// Escrita das respota da Tarefa_1A
int escritaResposta(FILE *csvDestino, int **matrizAdjacencia,
                    double maxDistanciasNormalizadas,
                    double minDistanciasNormalizadas, int iMaxNorm,
                    int jMaxNorm, int iMinNorm, int jMinNorm,
                    double maxDistanciasEuclidianas,
                    double minDistanciasEuclidianas, int iMaxEuc, int jMaxEuc,
                    int iMinEuc, int jMinEuc) {
  if (csvDestino != NULL) {
    fprintf(csvDestino, "150,%.2lf,%i,%i,%.2lf,%i,%i,%.2lf,%i,%i,%.2lf,%i,%i\n",
            maxDistanciasEuclidianas, iMaxEuc, jMaxEuc,
            minDistanciasEuclidianas, iMinEuc, jMinEuc,
            maxDistanciasNormalizadas, iMaxNorm, jMaxNorm,
            minDistanciasNormalizadas, iMinNorm, jMinNorm);
    for (int i = 0; i < 150; i++) {
      for (int j = i + 1; j < 150; j++) {
        if (matrizAdjacencia[i][j] == 1)
          fprintf(csvDestino, "%i,%i\n", i, j);
      }
    }
    fclose(csvDestino);
    return 1;
  } else {
    return 0;
  }
}

double calculaAcuracia(int fp, int tp, int fn, int tn){
  return (tp+tn)/(tp+tn+fn+fp);
}


double comparaPredicao( Flor *flores, int *ligacao ) {
  int fp = 0, tp = 0, fn = 0, tn = 0;
  for( int i = 0; i < 150; i++ ) {
    if( ligacao[i] == 1 ) {
      if( strcmp(flores[i].tipo, "Setosa") == 0 ) {
        tp++;
      } else {
        fp++;
      }
    } else {
      if( strcmp(flores[i].tipo, "Setosa") == 0 ) {
        fn++;
      } else {
        tn++;
      }
    }
  }
  return calculaAcuracia(fp, tp, fn, tn);
}

// Flor media(Flor *flores, int *ligacao){
//   int count = 0;
//   double somaSW = 0, somaSL = 0, somaPW = 0, somaPL = 0;
//   Flor retorno;
//   for (int i = 0; i < 150; i++)
//   {
//     if (ligacao[i] == 1)
//     {
//       somaSW += flores[i].sw;
//       somaSL += flores[i].sl;
//       somaPW += flores[i].pw;
//       somaPL += flores[i].pl;
//       count++;
//     }
//   }
//   retorno.sw = somaSW/count;
//   retorno.sl = somaSL/count;
//   retorno.pw = somaPW/count;
//   retorno.pl = somaPL/count;
  
//   return retorno;
// }

// int verificaPelaMedia(Flor aVerificar, Flor mediaSetosas, Flor mediaNSetosas){
//   double deSet = distEucl(&aVerificar, &mediaSetosas), deNset = distEucl(&aVerificar, &mediaNSetosas);
//   if (deSet > deNset)
//   {
//     return -1;
//   }else if (deSet < deNset)
//   {
//     return 1;
//   }else{
//     return 0;
//   }
  
  
// }
