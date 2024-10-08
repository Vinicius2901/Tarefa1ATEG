#include <stdio.h>
#include <stdlib.h>
#include "distancias.h"

int main() {
    int i, j;
    Flor flores[150];
    // Abertura e coleta dos dados do arquivo csv
    FILE *f = fopen("IrisDataset.csv", "rt");
    if( f != NULL ) {
        char lixo[50];
        fscanf(f, "%[^\n]", lixo );
        for( i = 0; i < 150; i++ ) {
            for( j = 0; j < 11; j++ ) {
                flores[i].tipo[j] = '\0';
            }
            fscanf(f, "\n%[^,],", flores[i].tipo); // Lê até 10 caracteres ou até a vírgula
            fscanf( f, "%lf,%lf,%lf,%lf", &flores[i].sl, &flores[i].sw, &flores[i].pl, &flores[i].pw );
        }
        fclose(f);

        // double distEuc[150][150], distEucNorm[150][150];
        // Inicialização da matriz de distâncias euclidianas e cálculo delas entre as flores
        double **distEuc = malloc( 150*sizeof(double*) );
        for( i = 0; i < 150; i++ ) {
            distEuc[i] = malloc(150*sizeof(double));
        }
        double aux = 0;
        int iMaxEuc = 0, jMaxEuc = 0, iMinEuc = 0, jMinEuc = 0;
        for( i = 0; i < 150; i++ ) {
            distEuc[i][i] = 0;
            for( j = i+1; j < 150; j++ ) {
                aux = distEucl( &flores[i], &flores[j] );
                distEuc[i][j] = aux;
                distEuc[j][i] = aux;
                // Pega a posição do máximo euclidiano encontrado e do mínimo, caso seja a primeira iteração
                if( aux > distEuc[iMaxEuc][iMinEuc] ) {
                    iMaxEuc = i;
                    jMaxEuc = j;
                    if( i == 0 ) {
                        iMinEuc = i;
                        jMinEuc = j;
                    }
                }
                // Pega a posição do minimo euclidiano
                if( aux < distEuc[iMinEuc][jMinEuc] ) {
                    iMinEuc = i;
                    jMinEuc = j;
                }
            }
        }

        // Inicialização da matriz de distâncias euclidianas normalizadas e cálculo delas entre as flores
        double distEucNorm[150][150];
        int iMaxNorm = 0, jMaxNorm = 0, iMinNorm = 0, jMinNorm = 0;
        for( i = 0; i < 150; i++ ) {
            distEucNorm[i][i] = 0;
            for( j = i+1; j < 150; j++ ) {
                aux = distEuclNorm( distEuc, i, j, distEuc[iMinEuc][jMinEuc], distEuc[iMaxEuc][jMaxEuc] );
                distEucNorm[i][j] = aux;
                distEucNorm[j][i] = aux;
                // Pega a posição do maior euclidiano normalizado e do menor, caso seja a primeira iteração
                if( aux > distEucNorm[iMaxNorm][jMaxNorm] ) {
                    iMaxNorm = i;
                    jMaxNorm = j;
                    if( i == 0 ) {
                        iMinNorm = i;
                        jMinNorm = j;
                    }
                }
                // Pega a posição do mínimo euclidiano normalizado
                if( aux < distEucNorm[iMinNorm][jMinNorm] ) {
                    iMinNorm = i;
                    jMinNorm = j;
                }
            }
        }

        // Inicialização da matriz adjacência e ligação dos nodos caso a distância euclidiana normalizada seja <= 0,3
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
                //printf("%i ", matrizAdj[i][j]);
            }
            //printf("\n");
        }
        FILE *data = fopen("data.csv", "wt");
        if( data != NULL ) {
            fprintf( data, "150,%.1lf,%i,%i,%.1lf,%i,%i,%.1lf,%i,%i,%.1lf,%i,%i\n", distEuc[iMaxEuc][jMaxEuc], iMaxEuc, jMaxEuc,
            distEuc[iMinEuc][jMinEuc], iMinEuc, jMinEuc, distEucNorm[iMaxNorm][jMaxNorm], iMaxNorm, jMaxNorm, distEucNorm[iMinNorm][jMinNorm]);
            for( i = 0; i < 150; i++ ) {
                for( j = i+1; j < 150; j++ ) {
                    if( matrizAdj[i][j] == 1 )
                        fprintf( data, "%i,%i\n", i, j);
                }
            }
            fclose(data);
        } else {
            printf("Erro ao computar os dados");
            return 1;
        }
    } else {
        printf("Erro na abertura de arquivo\n");
        return 1;
    }
    return 0;
}