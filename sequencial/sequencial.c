#include "sequencial.h"

int main(){
    //r = regiao (num de matrizes)
    //c = cidade (num de linhas)
    //a = alunos (num de colunas)
    int r, c, a, seed, nota;
    int *counter_cidade;
    int *counter_regiao = calloc(MAX_NOTA+1, sizeof(int));
    int *counter_brasil = calloc(MAX_NOTA+1, sizeof(int));
    int melhor_regiao[2] = {-1,-1}; //guarda a regiao e a media da melhor regiao
    int melhor_cidade[3] = {-1, -1, -1}; //guarda a regiao, media e num da melhor cidade
    int *somasEXl2, somaEX2lbr=0;
    float *somasMedias, somaMediabr=0;
    
    scanf("%d %d %d %d", &r, &c, &a, &seed);

    somasEXl2 = (int *) calloc(sizeof(int), r);
    somasMedias = (float *) calloc(sizeof(float), r);

    //primeira dimensao é a regiao, segunda é a cidade e terceira os alunos, guarda as notas deles
    int ***regiao = (int ***) malloc(sizeof(int **) * r);

    //struct com as infos da regiao
    Regiao *regioes = (Regiao *) calloc(sizeof(Regiao) , r);

    srand(seed);
    // populando a matriz com as notas
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

    double wtime = omp_get_wtime();

    for(int regiao_ = 0; regiao_ < r; regiao_++){
        for(int cidade_ = 0; cidade_ < c; cidade_++){
            //obtem o vetor count da cidade
            counter_cidade = count_notas(regiao, regiao_, cidade_, a);

            float media_cidade = media(regiao[regiao_][cidade_], a);

            //calculando o desvio padrao
            float EX2l_cidade = EX2l(counter_cidade, a); 
            somasEXl2[regiao_] += EX2l_cidade; 
            somasMedias[regiao_] += media_cidade;
            float dp_cidade = sqrt (((float)EX2l_cidade)/a - media_cidade*media_cidade);

            //verifica se a media da cidade é maior que a media da melhor cidade
            if (media_cidade > melhor_cidade[2]){
                melhor_cidade[0] = regiao_;
                melhor_cidade[1] = cidade_;
                melhor_cidade[2] = media_cidade;
            }

            //soma o vetor count da cidade ao vetor count da regiao
            soma_counters(counter_regiao, counter_cidade);

            regioes[regiao_].cidades[cidade_].media = media_cidade;
            regioes[regiao_].cidades[cidade_].dp = dp_cidade;
            registra_cidade(&regioes[regiao_].cidades[cidade_], counter_cidade, a);
            free(counter_cidade);        
        }
        
        float media_regiao = somasMedias[regiao_]/c;
        //verifica se a media da regiao é maior que a da melhor regiao
        if (media_regiao > melhor_regiao[1]){
            melhor_regiao[0] = regiao_;
            melhor_regiao[1] = media_regiao;
        }
        //calculo do desvio padrao da regiao
        regioes[regiao_].media = media_regiao;
        regioes[regiao_].dp = sqrt(somasEXl2[regiao_]/(c*a) - media_regiao*media_regiao);
        registra_regiao(&regioes[regiao_], counter_regiao, a);
        
        somaEX2lbr += somasEXl2[regiao_];
        somaMediabr += media_regiao;

        //soma o vetor count da regiao ao do brasil
        soma_counters(counter_brasil, counter_regiao);

        //zera o vetor count da regiao
        memset(counter_regiao, 0, (MAX_NOTA+1) * sizeof(int));
    }

    float mediaBr = somaMediabr /r;
    float dpBr = sqrt (somaEX2lbr/(r*c*a) - mediaBr*mediaBr);

    wtime = omp_get_wtime() - wtime;

    imprime_valores(regioes, r);
    printf("\nBrasil - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n\n", menor(counter_brasil), maior(counter_brasil), mediana(counter_brasil, a, c*r), mediaBr, dpBr);
    printf("Melhor região: Região %d\n", melhor_regiao[0]);
    printf("Melhor cidade: Região %d, Cidade %d\n", melhor_cidade[0], melhor_cidade[1]);

    printf ( "\nDone. Elapsed wall clock time = %.5f\n", wtime );

    //libera a memoria
    free(counter_brasil);
    free(counter_regiao);
    free(somasEXl2);
    free(somasMedias);
    for (int i=0; i<r; i++){
        destroy_regiao(&regioes[i]);
        for (int j=0; j<c; j++)
            free(regiao[i][j]);                
        free(regiao[i]);
    }
    free(regiao);
    free(regioes);
    return 0;
}