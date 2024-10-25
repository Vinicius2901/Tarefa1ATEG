#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef FUNCOES_H
#define FUNCOES_H
typedef struct flor {
  char tipo[11];
  double sl, sw, pl, pw;
} Flor;

typedef struct subgrafo {
  int tam;
  int *indices;
} Subgrafo;

typedef struct maiorSub {
  int tam1, tam2, indice1, indice2;
} MaiorSub;


double distEucl(Flor *f1, Flor *f2);

double distEuclNorm(double distEuc, double min,
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
                            double **distanciasNormalizadas, double limitante);

int testesMatrizAdjacencia(FILE *txtDestino, int **matrizAdjacencia);

int escritaResposta(FILE *csvDestino, int **matrizAdjacencia,
                    double maxDistanciasNormalizadas,
                    double minDistanciasNormalizadas, int iMaxNorm,
                    int jMaxNorm, int iMinNorm, int jMinNorm,
                    double maxDistanciasEuclidianas,
                    double minDistanciasEuclidianas, int iMaxEuc, int jMaxEuc,
                    int iMinEuc, int jMinEuc, double acuracia);
                    
int contaSetosa( Subgrafo *sub, Flor *flores );

double comparaPredicao( Flor *flores, int *ligacaoSetosas );

double calculaAcuracia(int fp, int tp, int fn, int tn);

Flor *media(Flor *flores, Subgrafo *ligacao);

// int verificaPelaMedia(Flor aVerificar, Flor mediaSetosas, Flor mediaNSetosas);
#endif
