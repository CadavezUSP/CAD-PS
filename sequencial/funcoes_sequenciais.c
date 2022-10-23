#include "sequencial.h"

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

    
void soma_counters (int *counter_regiao, int* counter_cidade){
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
    for (int i =0; i <= MAX_NOTA; i++){
        soma += count_notas[i] * i;
    }
    return (double) soma/alunos_total;
}
    
float mediana(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    int meio = (alunos_total)/2;
    int soma = 0;
    int i = 0;
    while (soma < meio){
        soma += count_notas[i];
        i++;
    }
    i--;
    int next_i = i+1;
    
    //numero de alunos par
    if (num_alunos % 2 == 0){
        if(count_notas[i] > 1){
            return (float) i;
        }
        while(count_notas[next_i] == 0){
                next_i++;
            }
        return (float)(i + i)/2;        
    }
    //impar
    else {
        return (float)i;
    }
}


void registra_cidade(Cidade *cidade, int* count_notas, int num_alunos){
    cidade->maior_nota = maior(count_notas);
    cidade->menor_nota = menor(count_notas);
    cidade->mediana = mediana(count_notas, num_alunos, 1);
}

void registra_regiao(Regiao *regiao, int* count_notas, int num_alunos){
    regiao->maior_nota = maior(count_notas);
    regiao->menor_nota = menor(count_notas);
    regiao->mediana = mediana(count_notas, num_alunos, regiao->num_cidades);
    regiao->dp = DP_counts(count_notas, num_alunos, regiao->num_cidades);
}

void destroy_regiao(Regiao *regiao){
    free(regiao->cidades);
}

float DP_counts(int* count_notas, int num_alunos, int multiplier){
    int alunos_total = num_alunos * multiplier;
    double soma = 0;
    double media_notas = media_counts(count_notas, num_alunos, multiplier);
    for (int i = 0; i <= MAX_NOTA; i++){
        if(count_notas[i] > 0){
            soma += (count_notas[i] * i - media_notas) * (count_notas[i] * i - media_notas);
        }
        // soma += (count_notas[i] * i - media_notas) * (count_notas[i] * i - media_notas);
    }
    return sqrt(soma/alunos_total);
}