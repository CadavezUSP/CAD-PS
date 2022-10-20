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

    for(int k = 0; k < r; k++){
        for(int i = 0; i < c; i++){
            for(int j = 0; j < a; j++){
                printf("%d ",regiao[k][i][j]);
            }
            printf("\n");
        }
            printf("\n");
            printf("\n");

    }

    for(int regiao_ = 0; regiao_ < r; regiao_++){
        for(int cidade_ = 0; cidade_ < c; cidade_++){
            //passa a regiao k, cidade i e num_alunos a
            counter_cidade = count_notas(regiao, regiao_, cidade_, a);
            float media_cidade = media(regiao[regiao_][cidade_], a);
            float dp_cidade = desvio_padrao(regiao[regiao_][cidade_], a);
            if (media_cidade > melhor_cidade[2]){
                melhor_cidade[0] = regiao_;
                melhor_cidade[1] = cidade_;
                melhor_cidade[2] = media_cidade;
            }
            soma_counters(counter_regiao, counter_cidade);
            // for (int i=0; i<=MAX_NOTA;i++){
            //     printf("%d ", counter_cidade[i]);
            // }
            regioes[regiao_].cidades[cidade_].media = media_cidade;
            regioes[regiao_].cidades[cidade_].dp = dp_cidade;
            registra_cidade(&regioes[regiao_].cidades[cidade_], counter_cidade, a);
            // printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", regiao_, cidade_, menor(counter_cidade), maior(counter_cidade), mediana(counter_cidade, a, 1), media_cidade, dp_cidade);
            free(counter_cidade);        
        }
        
        float media_regiao = media_counts(counter_regiao, a, c);
        if (media_regiao > melhor_regiao[1]){
            melhor_regiao[0] = regiao_;
            melhor_regiao[1] = media_regiao;
        }
        regioes[regiao_].media = media_regiao;
        registra_regiao(&regioes[regiao_], counter_regiao, a);
        // printf("\nReg %d - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", regiao_, menor(counter_regiao), maior(counter_regiao), mediana(counter_regiao, a, c), media_regiao, DP_counts(counter_regiao, a, c));
        
        soma_counters(counter_brasil, counter_regiao);
        memset(counter_regiao, 0, (MAX_NOTA+1) * sizeof(int));
        printf("\n");
    }
    // for (int i=0; i<MAX_NOTA;i++){
    //     printf("%d ", counter_brasil[i]);
    // }
    for (int i =0; i<r; i++){
        for (int j =0; j<c; j++){
            printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, j, regioes[i].cidades[j].menor_nota, regioes[i].cidades[j].maior_nota, regioes[i].cidades[j].mediana, regioes[i].cidades[j].media, regioes[i].cidades[j].dp);
        }
        printf("Reg %d - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, regioes[i].menor_nota, regioes[i].maior_nota, regioes[i].mediana, regioes[i].media, regioes[i].dp);
        printf("\n");
    }
    printf("\nBrasil - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n\n", menor(counter_brasil), maior(counter_brasil), mediana(counter_brasil, a, c*r), media_counts(counter_brasil, a, c*r), DP_counts(counter_brasil, a, c*r));
    printf("Melhor região: Região %d\n", melhor_regiao[0]);
    printf("Melhor cidade: Região %d, Cidade %d\n", melhor_cidade[0], melhor_cidade[1]);
    free(counter_brasil);
    free(counter_regiao);
    for (int i=0; i<r; i++){
        destroy_regiao(&regioes[i]);
        for (int j=0; j<c; j++){
            free(regiao[i][j]);
        }
        free(regiao[i]);
    }
    free(regioes);
}