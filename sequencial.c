#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include <string.h>
#include <math.h>

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
    for (int i=0; i<100;i++){
        counter_regiao[i] += counter_cidade[i];
    }
}

//obtem o numero de alunos que tirou cada nota. Ex nota[100] = 2 => 2 alunos tiraram nota 100
int * ordenar_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos){
    int *count_notas = (int*) calloc(100, sizeof(int));
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

int maior_nota(int *notas){

}

int main(){
    //r = regiao (num de matrizes)
    //c = cidade (num de linhas)
    //a = alunos (num de colunas)
    int r, c, a, seed, nota;
    int *counter_cidade;
    int *counter_regiao = calloc(100, sizeof(int));
    int *counter_brasil = calloc(100, sizeof(int));
    
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

            soma_counters(counter_regiao, counter_cidade);
            free(counter_cidade);
            printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %d, média: %.2f e DP: %.2f\n", regiao_, cidade_, regiao[regiao_][cidade_][0], regiao[regiao_][cidade_][a - 1], regiao[regiao_][cidade_][(a - 1)/2], media_cidade, dp_cidade);
        }
        for (int i=0; i<100;i++){
            printf("%d ", counter_regiao[i]);
        }
        soma_counters(counter_brasil, counter_regiao);
        memset(counter_regiao, 0, 100 * sizeof(int));
        printf("\n");
    }
    for (int i=0; i<100;i++){
        printf("%d ", counter_brasil[i]);
    }

}