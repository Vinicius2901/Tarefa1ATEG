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
    //   FILE *teste = fopen("distEuclidianas.txt", "wt");
    //   if (testesDistanciaEuclidiana(teste, distEuc) == 0)
    //   {
    //     printf("Erro na abertura do arquivo!\n");
    //     return 1;
    //   }

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
    
    //   FILE *teste2 = fopen("distEuclidianasNormalizadas.txt", "wt");
    //   if (testesNormalizacao(teste2, distanciasEuclidianasNormalizadas) == 0)
    //   {
    //     printf("Erro na abertura do arquivo!\n");
    //     return 1;
    //   }

    // Inicialização da matriz adjacência e ligação dos nodos caso a distância
    // euclidiana normalizada seja <= 0,2
    int **matrizAdj;
    matrizAdj = malloc(150 * sizeof(int *));
    for (int i = 0; i < 150; i++)
    {
        matrizAdj[i] = calloc(150, sizeof(int));
    }
    montarMatrizAdjacencia(matrizAdj, distanciasEuclidianasNormalizadas, 0.05);


    // Teste matriz adjacencia

    // FILE *mAdj = fopen("matrizAdj.txt", "wt");
    // if (testesMatrizAdjacencia(mAdj, matrizAdj) == 0)
    // {
    //     printf("Erro na abertura do arquivo!\n");
    //     return 1;
    // }


    // Realizacao da dfs para verificar se existe conexao entre flores
    // Setosas
    int *vetSub, *vetContido;
    vetSub = calloc(150, sizeof(int));
    vetContido = calloc(150, sizeof(int));
    // dfs(0, matrizAdj, vetSub);

    // // Não setosas
    // int *vetSub2;
    // vetSub2 = calloc(150, sizeof(int));
    // dfs(50, matrizAdj, vetSub2);

    int contPass = 0, novoSubGraf = 0,  // contPass é por quantos vértices passamos, novoSubGraf é a quantidade de subgrafos criados
    indiceInicial = 0;                  // indiceInicial é o primeiro índice do subgrafo
    Subgrafo **listaNovos = NULL;       // Lista de subgrafos criados com a normalização <= 0.2
    while( contPass < 150 ) {
        listaNovos = realloc( listaNovos, (novoSubGraf+1)*(sizeof(Subgrafo*)) );    // Realocação da quantidade de subgrafos
        Subgrafo *novo = malloc(sizeof(Subgrafo));                                  // Alocação do novo subgrafo
        novo->tam = 0;
        novo->indices = malloc( 150*sizeof(int));

        for( indiceInicial = 0; vetContido[indiceInicial] == 1; indiceInicial++ );  // Determinação do indice inicial do subgrafo

        dfs( indiceInicial, matrizAdj, vetSub );                                    // Determinação do subgrafo (Os índices que foram passados )


        for( int i = 0; i < 150; i++ ) {
            if( vetSub[i] == 1 ) {
                vetContido[i] = 1;                                                  // Marca a posição que foi passada
                contPass++;                                                         // Atualiza a quantidade de vértices que passamos
                (novo->tam)++;                                                      // Atualiza o tamanho do vetor
                // novo->indices = realloc( novo->indices, (novo->tam)*sizeof(int) );  // Realoca memória
                novo->indices[novo->tam-1] = i;                                     // Guarda a posição que passa o subgrafo
            }
            vetSub[i] = 0;                                                          // Reinicializa a posição do subgrafo para 0;
        }
        listaNovos[novoSubGraf] = novo;                                             // Coloca o novo subgrafo na lista de subgrafos
        novoSubGraf++;                                                              // Atualiza o valor da quantidade de subgrafos
    }

    MaiorSub *maiores = malloc(sizeof(MaiorSub));
    maiores->tam1 = listaNovos[0]->tam; maiores->indice1 = 0;
    maiores->tam2 = listaNovos[1]->tam; maiores->indice2 = 1;
    for( int i = 2; i < novoSubGraf; i++ ) {
        if( maiores->tam1 > maiores->tam2 ) {
            if( maiores->tam2 < listaNovos[i]->tam ) {
                maiores->tam2 = listaNovos[i]->tam;
                maiores->indice2 = i;
            }
        } else {
            if( maiores->tam1 < listaNovos[i]->tam ) {
                maiores->tam1 = listaNovos[i]->tam;
                maiores->indice1 = i;
            }
        }
    }
    Flor *media1 = media( flores, listaNovos[maiores->indice1]);
    Flor *media2 = media( flores, listaNovos[maiores->indice2]);
    for( int i = 0; i < novoSubGraf; i++ ) {
        if( i != maiores->indice1 && i != maiores->indice2 ) {
            if( distEucl( media1, &flores[listaNovos[i]->indices[0]]) < distEucl( media2, &flores[listaNovos[i]->indices[0]]) ) {
                for( int j = 0; j < listaNovos[i]->tam; j++ ) {
                    listaNovos[maiores->indice1]->indices[listaNovos[maiores->indice1]->tam] = listaNovos[i]->indices[j];
                    (listaNovos[maiores->indice1]->tam)++;
                }
            } else {
                for( int j = 0; j < listaNovos[i]->tam; j++ ) {
                    listaNovos[maiores->indice2]->indices[listaNovos[maiores->indice2]->tam] = listaNovos[i]->indices[j];
                    (listaNovos[maiores->indice2]->tam)++;
                }
            }
        }
    }
    
    int setosa1 = contaSetosa(listaNovos[maiores->indice1], flores);
    int setosa2 = contaSetosa(listaNovos[maiores->indice2], flores);

    for( int i = 0; i < 150; i++ ) {
        vetContido[i] = 0;
    }
    if( setosa1 > setosa2 ) {
        for( int i = 0; i < listaNovos[maiores->indice1]->tam; i++ ) {
            vetContido[listaNovos[maiores->indice1]->indices[i]] = 1;
        }
    } else {
        for( int i = 0; i < listaNovos[maiores->indice2]->tam; i++ ) {
            vetContido[listaNovos[maiores->indice2]->indices[i]] = 1;
        }
    }

    // for( int i = 0; i < novoSubGraf; i++ ) {
    //     printf("%i: ");
    //     for( int j = 0; j < listaNovos[i]->tam; j++ ) {
    //         printf("%i ", listaNovos[i]->indices[j]);
    //     }
    //     printf("\n");
    // }

    for( int i = 0; i < 150; i++ ) {
        printf("%i ", vetContido[i]);
    }
    printf("\n");

    // Cálculo da acurácia
    double acc = comparaPredicao(flores, vetContido);

    // Escrita do csv de resposta da Tarefa_1A
    FILE *data = fopen("data.csv", "wt");
    if (escritaResposta(data, matrizAdj,
                        distanciasEuclidianasNormalizadas[iMaxNorm][jMaxNorm],
                        distanciasEuclidianasNormalizadas[iMinNorm][jMinNorm],
                        iMaxNorm, jMaxNorm, iMinNorm, jMinNorm,
                        distEuc[iMaxEuc][jMaxEuc], distEuc[iMinEuc][jMinEuc],
                        iMaxEuc, jMaxEuc, iMinEuc, jMinEuc, acc) == 0)
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
    // Flor mSet = media(setosas, vetSub);
    // Flor mNSet = media(nSetosas,vetSub2);
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
    //     vetSub[i] = 1;
    //     h++;
    //   }else
    //   {
    //     strcpy(floresPredicoes[i].tipo, auxN);
    //     nSetosas[k] = flores[i];
    //     vetSub2[i] = 1;
    //     k++;
    //   }
    //   mSet = media(setosas,vetSub);
    //   mNSet = media(nSetosas, vetSub2);
    //   printf("%s\n", floresPredicoes[i].tipo);
    // }
    
    // printf("%.2lf\n", comparaPredicao(floresPredicoes, vetSub));


    // Teste se o grupo de não setosas está conexo

    // FILE *teste4 = fopen("marcasRaiz50.csv", "wt");
    // if (teste4 != NULL)
    // {
    //     for (int i = 1; i < 150; i++)
    //     {
    //     fprintf(teste4, "%d\n", vetSub2[i]);
    //     }
    // }


    // Teste se o grupo de Setosas está conexo

    // FILE *testes3 = fopen("marcasRaiz0.csv", "wt");
    // if (testes3 != NULL)
    // {
    //     for (int i = 1; i < 150; i++)
    //     {
    //     fprintf(testes3, "%d\n", vetSub[i]);
    //     }
    // }

    // Liberacao de memoria
    for (int i = 0; i < 150; i++)
    {
        free(distEuc[i]);
        free(distanciasEuclidianasNormalizadas[i]);
        free(matrizAdj[i]);
    } 
    for( int i = 0; i < novoSubGraf; i++ ) {
        free(listaNovos[i]);
    }
    free(matrizAdj);
    free(distEuc);
    free(distanciasEuclidianasNormalizadas);
    free(flores);
    free(listaNovos);
    // free(floresPredicoes);
    return 0;
    }
