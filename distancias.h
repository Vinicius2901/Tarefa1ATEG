#include <math.h>

#ifndef DISTANCIAS
#define DISTANCIAS
typedef struct flor {
    char tipo[11];
    double sl, sw, pl, pw;
} Flor;

double distEucl( Flor *f1, Flor *f2 );

double distEuclNorm(  double **distEuc, int linha, int coluna, double min, double max );

#endif