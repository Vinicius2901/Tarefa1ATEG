#include <stdio.h>
#include <stdlib.h>
#include "distancias.h"

int main() {
    int i, j;
    Flor flores[150];
    FILE *f = fopen("C:\\Users\\vinic\\OneDrive\\Documents\\UDESC\\4 Semestre\\TEG\\PercursosECaminhos\\Tarefa1A\\IrisDataset.csv", "rt");
    if( f != NULL ) {
        char lixo[50];
        fscanf(f, "%[^\n]", lixo );
        for( i = 0; i < 150; i++ ) {
            for( j = 0; j < 11; j++ ) {
                flores[i].tipo[j] = '\0';
            }
            fscanf(f, "\n%10[^,],", flores[i].tipo); // Lê até 10 caracteres ou até a vírgula
            fscanf( f, "%lf,%lf,%lf,%lf", &flores[i].sl, &flores[i].sw, &flores[i].pl, &flores[i].pw );
        }
        fclose(f);

    // double distEuc[150][150], distEucNorm[150][150];
    double **distEuc = malloc( 150*sizeof(double*) );
    for( i = 0; i < 150; i++ ) {
        distEuc[i] = malloc(150*sizeof(double));
    }
    double distEucNorm[150][150];
    double aux = 0, max = 0, min = 0;
    for( i = 0; i < 150; i++ ) {
        distEuc[i][i] = 0;
        for( j = i+1; j < 150; j++ ) {
            aux = distEucl( &flores[i], &flores[j] );
            distEuc[i][j] = aux;
            distEuc[j][i] = aux;
            if( aux > max ) {
                max = aux;
                if( i == 0 ) {
                    min = aux;
                }
            }
            if( aux < min ) {
                min = aux;
            }
        }
    }
    for( i = 0; i < 150; i++ ) {
        distEucNorm[i][i] = 0;
        for( j = i+1; j < 150; j++ ) {
            aux = distEuclNorm( distEuc, i, j, min, max );
            distEucNorm[i][j] = aux;
            distEucNorm[j][i] = aux;
            if( aux > max ) {
                max = aux;
                if( i == 0 ) {
                    min = aux;
                }
            }
            if( aux < min ) {
                min = aux;
            }
        }
    }
    int matrizAdj[150][150];
    for( i = 0; i < 150; i++ ) {
        matrizAdj[i][i] = 0;
        for( j = i+1; j < 150; j++ ) {
            if( distEucNorm[i][j] <= 0.3 ) {
                matrizAdj[i][j] = 1;
                matrizAdj[j][i] = 1;
            } else {
                matrizAdj[i][j] = 0;
                matrizAdj[j][i] = 0;
            }
            printf("%.1lf ", distEuc[i][j]);
        }
        printf("\n");
    }
    } else {
        printf("Erro na abertura de arquivo\n");
        return 1;
    }
}