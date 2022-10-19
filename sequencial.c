#include "sequencial.h"


int main(){
    //r = regiao (num de matrizes)
    //c = cidade (num de linhas)
    //a = alunos (num de colunas)
    int r, c, a, seed, nota;
    int *counter_cidade;
    int *counter_regiao = calloc(MAX_NOTA+1, sizeof(int));
    int *counter_brasil = calloc(MAX_NOTA+1, sizeof(int));
    int melhor_regiao[2] = {-1,-1};
    int melhor_cidade[3] = {-1, -1, -1};
    
    scanf("%d %d %d %d", &r, &c, &a, &seed);

    int ***regiao = (int ***) malloc(sizeof(int **) * r);

    srand(seed);
    for(int k = 0; k < r; k++){
        regiao[k] = (int **) malloc(sizeof(int *) * c);
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

    for(int regiao_ = 0; regiao_ < r; regiao_++){
        for(int cidade_ = 0; cidade_ < c; cidade_++){
            //passa a regiao k, cidade i e num_alunos a
            counter_cidade = ordenar_notas(regiao, regiao_, cidade_, a);
            float media_cidade = media(regiao[regiao_][cidade_], a);
            float dp_cidade = desvio_padrao(regiao[regiao_][cidade_], a);
            if (media_cidade > melhor_cidade[2]){
                melhor_cidade[0] = regiao_;
                melhor_cidade[1] = cidade_;
                melhor_cidade[2] = media_cidade;
            }
            soma_counters(counter_regiao, counter_cidade);
            free(counter_cidade);
            printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", regiao_, cidade_, regiao[regiao_][cidade_][0], regiao[regiao_][cidade_][a - 1], mediana(regiao[regiao_][cidade_], a), media_cidade, dp_cidade);
        }
        // for (int i=0; i<=MAX_NOTA;i++){
        //     printf("%d ", counter_regiao[i]);
        // }
        float media_regiao = media_counts(counter_regiao, a, c);
        if (media_regiao > melhor_regiao[1]){
            melhor_regiao[0] = regiao_;
            melhor_regiao[1] = media_regiao;
        }
        printf("\nReg %d - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", regiao_, menor(counter_regiao), maior(counter_regiao), mediana_counts(counter_regiao,a, c), media_regiao, DP_counts(counter_regiao, a, c));
        
        soma_counters(counter_brasil, counter_regiao);
        memset(counter_regiao, 0, (MAX_NOTA+1) * sizeof(int));
        printf("\n");
    }
    // for (int i=0; i<MAX_NOTA;i++){
    //     printf("%d ", counter_brasil[i]);
    // }
    printf("\nBrasil - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n\n", menor(counter_brasil), maior(counter_brasil), mediana_counts(counter_brasil, a, c*r), media_counts(counter_brasil, a, c*r), DP_counts(counter_brasil, a, c*r));
    printf("Melhor região: Região %d\n", melhor_regiao[0]);
    printf("Melhor cidade: Região %d, Cidade %d\n", melhor_cidade[0], melhor_cidade[1]);
}