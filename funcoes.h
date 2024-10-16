#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef DISTANCIAS
#define DISTANCIAS
typedef struct flor {
  char tipo[11];
  double sl, sw, pl, pw;
} Flor;

double distEucl(Flor *f1, Flor *f2);

double distEuclNorm(double **distEuc, int linha, int coluna, double min,
                    double max);

void dfs(int raiz, int **matAdj, int *vetContido);

int leituraIrisDataset(Flor *flores, FILE *arquivoCSV);

void montagemDistanciaEuclidiana(double **distEuc, Flor *flores,
                                 int *linhaMaximoEuclidiano,
                                 int *colunaMaximoEuclidiano,
                                 int *linhaMinimoEuclidiano,
                                 int *colunaMinimoEuclidiano);

int testesDistanciaEuclidiana(FILE *txtDestino, double **distanciasEuclidianas);

void montagemNormalizacaoDistanciasEuclidianas(
    double **distanciasNormalizadas, double **distanciasEuclidianas,
    int *linhaMaximoNormalizado, int *colunaMaximoNormalizado,
    int *linhaMinimoNormalizado, int *colunaMinimoNormalizado,
    double minimoEuclidiano, double maximoEuclidiano);

int testesNormalizacao(FILE *txtDestino, double **distanciasNormalizadas);

void montarMatrizAdjacencia(int **matrizAdjacencia,
                            double **distanciasNormalizadas);

int testesMatrizAdjacencia(FILE *txtDestino, int **matrizAdjacencia);

int escritaResposta(FILE *csvDestino, int **matrizAdjacencia,
                    double maxDistanciasNormalizadas,
                    double minDistanciasNormalizadas, int iMaxNorm,
                    int jMaxNorm, int iMinNorm, int jMinNorm,
                    double maxDistanciasEuclidianas,
                    double minDistanciasEuclidianas, int iMaxEuc, int jMaxEuc,
                    int iMinEuc, int jMinEuc);
#endif
