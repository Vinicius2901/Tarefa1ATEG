#include "distancias.h"

double distEucl( Flor *f1, Flor *f2 ) {
    return sqrt((f2->sl - f1->sl)*(f2->sl - f1->sl) + (f2->sw - f1->sw)*(f2->sw - f1->sw) + (f2->pl - f1->pl)*(f2->pl - f1->pl) + (f2->pw - f1->pw)*(f2->pw - f1->pw) );
}

double distEuclNorm(  double **distEuc, int linha, int coluna, double min, double max ) {
    return (distEuc[linha][coluna]-min)/(max-min);
}