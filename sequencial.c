#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include <math.h>
#define MAX_NOTA 100


float media (int *notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i < num_alunos; i++)
        soma += notas[i];
    return soma/num_alunos;
}

float desvio_padrao (int *notas, int num_alunos){
    int i;
    float soma = 0;
    float media_notas = media(notas, num_alunos);
    for (i = 0; i < num_alunos; i++)
        soma += (notas[i] - media_notas) * (notas[i] - media_notas);
    return sqrt(soma/num_alunos);
}

float mediana(int *notas, int num_alunos){
    int i;
    int meio = ((num_alunos)/2);
    // printf("meio: %d", meio);
    float mediana;
    if(meio == 1){
        mediana = notas[meio];
    }
    else if(num_alunos % 2 == 0){
        mediana = (notas[meio] + notas[meio - 1])/2;
    }
    else {
        mediana = notas[meio];
    }
    return mediana;
}

void counting_sort(int *notas, int num_alunos){
    int i, j, count;
    int *aux = (int *) malloc(num_alunos * sizeof(int));
    for (i = 0; i < num_alunos; i++) {
        count = 0;
        for (j = 0; j < num_alunos; j++) {
            if (notas[j] < notas[i])
                count++;
            else if (notas[j] == notas[i] && j < i)
                count++;
        }
        aux[count] = notas[i];
    }
    for (i = 0; i < num_alunos; i++)
        notas[i] = aux[i];
    free(aux);
}
    
void soma_counters (int *counter_regiao, int* counter_cidade){
    for (int i=0; i<=MAX_NOTA;i++){
        counter_regiao[i] += counter_cidade[i];
    }
}

//obtem o numero de alunos que tirou cada nota. Ex nota[MAX_NOTA] = 2 => 2 alunos tiraram nota MAX_NOTA
int * ordenar_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos){
    int *count_notas = (int*) calloc(MAX_NOTA+1, sizeof(int));
    for (int i =0;i<num_alunos;i++){
        count_notas[matriz_regioes[regiao][cidade][i]]++;
    }
    printf("\n");
    int *notas_cidade = (int *) malloc(sizeof(int) * num_alunos);
    memcpy(notas_cidade, matriz_regioes[regiao][cidade], sizeof(int) * num_alunos);
    counting_sort(notas_cidade, num_alunos);
    memcpy(matriz_regioes[regiao][cidade], notas_cidade, sizeof(int) * num_alunos);
    // for (int i = 0; i < num_alunos; i++){
    //     printf("%d ", matriz_regioes[regiao][cidade][i]);
    // }
    // printf("\n");
    free(notas_cidade);
    return count_notas;
}

int maior(int* count_notas){
    int maior = 0;
    for (int i = MAX_NOTA; i >=0; i--){
        if (count_notas[i] > 0){
            maior = i;
            break;
        }
    }
    return maior;
}

int menor(int* count_notas){
    int menor = 500;
    for (int i = 0; i <= MAX_NOTA; i++){
        if (count_notas[i] > 0){
            menor = i;
            break;
        }
    }
    return menor;
}


double media_counts(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    int soma = 0;
    for (int i =0; i <= MAX_NOTA; i++){
        soma += count_notas[i] * i;
    }
    // printf("soma: %d, alunos_total: %d\n", soma, alunos_total);
    return (double) soma/alunos_total;
}
    
float mediana_counts(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    int meio = (alunos_total)/2;
    int soma = 0;
    int i = 0;
    // printf("meio: %d", meio);
    while (soma < meio){
        soma += count_notas[i];
        i++;
    }
    i--;
    int next_i = i+1;
    while (count_notas[next_i] == 0){
        next_i++;
    }
    
    if (soma == meio){
        // printf("i: %d\n prev_i: %d", i, next_i);
        return (float)(i + next_i)/2;
    }
    else {
        return (float)i;
    }
}


float DP_counts(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    double soma = 0;
    double media_notas = media_counts(count_notas, num_alunos, multiplier);
    for (int i = 0; i <= MAX_NOTA; i++){
        if(count_notas[i] > 0){
            soma += (count_notas[i] * i - media_notas) * (count_notas[i] * i - media_notas);
        // printf("%f\n", count_notas[i] * i - media_notas);
        }
        // soma += (count_notas[i] * i - media_notas) * (count_notas[i] * i - media_notas);
    }
    // printf("soma: %f, alunos_total: %d\n", soma, alunos_total);
    return sqrt(soma/alunos_total);
}

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