#include "sequencial.h"

//funcao que calcula E[x²] para o desvio padrao 
float EX2l (int *count_notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i <= MAX_NOTA; i++)
        soma += count_notas[i]*i*i;
    return soma;
}

//calcula a media
float media (int *notas, int num_alunos){
    int i;
    float soma = 0;
    for (i = 0; i < num_alunos; i++)
        soma += notas[i];
    return soma/num_alunos;
}

//soma o vetor counter2 ao vetor counter1
void soma_counters (int *counter1, int* counter2){
    for (int i=0; i<=MAX_NOTA;i++){
        counter1[i] += counter2[i];
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

//obtem a maior nota
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

//obtem a menor nota
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
        return (float)(next_i + i)/2;        
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

void imprime_valores(Regiao *regioes, int r){
    for (int i =0; i<r; i++){
        for (int j =0; j<regioes[i].num_cidades; j++){
            printf("Reg %d - Cid %d: menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, j, regioes[i].cidades[j].menor_nota, regioes[i].cidades[j].maior_nota, regioes[i].cidades[j].mediana, regioes[i].cidades[j].media, regioes[i].cidades[j].dp);
        }
        printf("Reg %d - menor: %d, maior: %d, mediana: %.2f, média: %.2f e DP: %.2f\n", i, regioes[i].menor_nota, regioes[i].maior_nota, regioes[i].mediana, regioes[i].media, regioes[i].dp);
        printf("\n");
    }
}