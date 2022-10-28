#include "paralelo.h"
#include "omp.h"


float EX2l (int *count_notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i <= MAX_NOTA; i++)
        soma += count_notas[i]*i*i;
    return soma;
}

float media (int *count_notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i <= MAX_NOTA; i++)
        soma += count_notas[i]*i;
    return soma/num_alunos;
}

float EX2 (int *count_notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i <= MAX_NOTA; i++)
        soma += count_notas[i]*i*i;
    return soma/num_alunos;
}

    
void soma_counters (int *counter_regiao, int* counter_cidade){
    #pragma omp simd
    for (int i=0; i<=MAX_NOTA;i++){
        counter_regiao[i] += counter_cidade[i];
    }
}

//obtem o numero de alunos que tirou cada nota. Ex nota[MAX_NOTA] = 2 => 2 alunos tiraram nota MAX_NOTA
int * count_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos){
    int *count_notas = (int*) calloc(MAX_NOTA+1, sizeof(int));
    for (int i =0;i<num_alunos;i++){
        count_notas[matriz_regioes[regiao][cidade][i]]++;
    }
    return count_notas;
}

int maior(int* count_notas){
    int maior = 0;
    for (int i = MAX_NOTA; i >=0; i--){
        if (count_notas[i] != 0){
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
    #pragma omp parallel for schedule(guided) reduction(+: soma) num_threads(NUM_THREADS)
    for (int i =0; i <= MAX_NOTA; i++){
        soma += count_notas[i] * i;
    }
    return (double) soma/alunos_total;
}
    
//obtem a mediana das notas
float mediana(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    int meio = (alunos_total)/2;
    int soma = 0;
    int i = 0;
    while (soma < meio){
        soma += count_notas[i];
        i++;
    }
    if (num_alunos != 3)
        i--;

    int next_i = i+1;
    //numero de alunos par
    if (num_alunos * multiplier % 2 == 0){
        if(count_notas[i] > 1){
            return (float) i;
        }
        while(count_notas[next_i] == 0){
                next_i++;
            }
        return (float)(next_i + i)/2;        
    }
    //impar
    else {
         while(count_notas[next_i] == 0){
                next_i++;
            }
        return (float)next_i;
    }
}


void destroy_regiao(Regiao *regiao){
    free(regiao->cidades);
}


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
