#include "paralelo.h"


void imprime_valores(Regiao *regioes, int r){
    for (int i =0; i<r; i++){
        for (int j =0; j<regioes[i].num_cidades; j++){
            printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, j, regioes[i].cidades[j].menor_nota, regioes[i].cidades[j].maior_nota, regioes[i].cidades[j].mediana, regioes[i].cidades[j].media, regioes[i].cidades[j].dp);
        }
        printf("Reg %d - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, regioes[i].menor_nota, regioes[i].maior_nota, regioes[i].mediana, regioes[i].media, regioes[i].dp);
        printf("\n");
    }
}

void maxCidade(int *omp_out, int *omp_in) {
    if (omp_in[2] > omp_out[2]) {
        omp_out[0] = omp_in[0];
        omp_out[1] = omp_in[1];
        omp_out[2] = omp_in[2];
    }
}

int main(){
    //r = regiao (num de matrizes)
    //c = cidade (num de linhas)
    //a = alunos (num de colunas)
    int r, c, a, seed, nota;
    int *counter_cidade;
    int *counter_brasil = calloc(MAX_NOTA+1, sizeof(int));
    // int melhor_regiao[2] = {-1,-1};
    // int melhor_cidade[3] = {-1, -1, -1};
    int *somasEXl2, somaEX2lbr=0;
    float *somasMedias, somaMediabr=0;
    MyMax myMaxCidade, myMaxRegiao;
    myMaxCidade.maxMedia = 0;
    myMaxRegiao.maxMedia = 0;
    
    scanf("%d %d %d %d", &r, &c, &a, &seed);

    somasEXl2 = (int *) calloc(sizeof(int), r);
    somasMedias = (float *) calloc(sizeof(float), r);

    int ***regiao = (int ***) malloc(sizeof(int **) * r);
    Regiao *regioes = (Regiao *) calloc(sizeof(Regiao) , r);

    srand(seed);
    for(int k = 0; k < r; k++){
        regiao[k] = (int **) malloc(sizeof(int *) * c);
        regioes[k].cidades = (Cidade *) calloc(sizeof(Cidade), c);
        regioes[k].num_cidades = c;
        for(int i = 0; i < c; i++){
            regiao[k][i] = (int *) malloc(sizeof(int) * a);
            for(int j = 0; j < a; j++){
                nota = rand() % 101;
                regiao[k][i][j] = nota;
            }
        }
    }

    // for(int k = 0; k < r; k++){
    //     for(int i = 0; i < c; i++){
    //         for(int j = 0; j < a; j++){
    //             printf("%d ",regiao[k][i][j]);
    //         }
    //         printf("\n");
    //     }
    //         printf("\n");
    //         printf("\n");

    // }

    double wtime = omp_get_wtime();

    #pragma omp declare reduction(maxCidade : int *: \
    maxCidade(omp_out, omp_in))

    #pragma omp declare reduction(maximo : MyMax : omp_out = omp_in.maxMedia > omp_out.maxMedia ? omp_in : omp_out)

    omp_set_nested(1);

    #pragma omp parallel for schedule(guided) reduction(+: somaEX2lbr) reduction(+: somaMediabr) \
        reduction(+:counter_brasil[:MAX_NOTA]) reduction(maximo: myMaxRegiao) num_threads(NUM_THREADS)
    for(int regiao_ = 0; regiao_ < r; regiao_++){
        int *meu_counter_regiao = calloc(MAX_NOTA+1, sizeof(int));
        #pragma omp parallel for private(counter_cidade) schedule(guided) reduction(+: somasEXl2[regiao_]) reduction(+:somasMedias[regiao_]) \
            reduction(+: meu_counter_regiao[:MAX_NOTA]) reduction(maximo:myMaxCidade) num_threads(NUM_THREADS)
        for(int cidade_ = 0; cidade_ < c; cidade_++){

            // printf("%d ", omp_get_thread_num());

            counter_cidade = count_notas(regiao, regiao_, cidade_, a);
            float media_cidade;
            float EX2l_cidade;
            media_cidade = media(counter_cidade, a);
            EX2l_cidade = EX2l(counter_cidade, a); 

            registra_cidade(&regioes[regiao_].cidades[cidade_], counter_cidade, a);

            regioes[regiao_].cidades[cidade_].media = media_cidade;
            regioes[regiao_].cidades[cidade_].dp = sqrt (((float)EX2l_cidade)/a - media_cidade*media_cidade);

            somasEXl2[regiao_] += EX2l_cidade; 
            somasMedias[regiao_] += media_cidade;

            if (media_cidade > myMaxCidade.maxMedia){
                myMaxCidade.maxMedia = media_cidade;
                myMaxCidade.regiao = regiao_;
                myMaxCidade.cidade = cidade_;
            }

            soma_counters(meu_counter_regiao, counter_cidade);

            free(counter_cidade);        
        }
        
        float media_regiao = somasMedias[regiao_]/c;

        // #pragma omp critical
        // {
        //     if (media_regiao > melhor_regiao[1]){
        //         melhor_regiao[0] = regiao_;
        //         melhor_regiao[1] = media_regiao;
        //     }
        //     soma_counters(counter_brasil, meu_counter_regiao);
        // }
        if (media_regiao > myMaxRegiao.maxMedia){
            myMaxRegiao.maxMedia = media_regiao;
            myMaxRegiao.regiao = regiao_;
        }
        soma_counters(counter_brasil, meu_counter_regiao);

        regioes[regiao_].media = media_regiao;
        regioes[regiao_].dp = sqrt(somasEXl2[regiao_]/(c*a) - media_regiao*media_regiao);
        registra_regiao(&regioes[regiao_], meu_counter_regiao, a);
        
        somaEX2lbr += somasEXl2[regiao_];
        somaMediabr += media_regiao;

        free(meu_counter_regiao);
    }

    float mediaBr = somaMediabr /r;
    float dpBr = sqrt (somaEX2lbr/(r*c*a) - mediaBr*mediaBr);

    wtime = omp_get_wtime() - wtime;

    // imprime_valores(regioes, r);
    // printf("\nBrasil - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n\n", menor(counter_brasil), maior(counter_brasil), mediana(counter_brasil, a, c*r), mediaBr, dpBr);
    // printf("Melhor região: Região %d\n", myMaxRegiao.regiao);
    // printf("Melhor cidade: Região %d, Cidade %d\n", myMaxCidade.regiao, myMaxCidade.cidade);
    // printf("done\n");

    printf ( "\nDone. Elapsed wall clock time = %.5f\n", wtime );

    free(counter_brasil);
    for (int i=0; i<r; i++){
        destroy_regiao(&regioes[i]);
        for (int j=0; j<c; j++){
            free(regiao[i][j]);
        }
        free(regiao[i]);
    }
    free(regioes);
}