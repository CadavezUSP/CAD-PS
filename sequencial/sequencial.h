#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#define MAX_NOTA 100

typedef struct {
    int maior_nota;
    int menor_nota;
    float media;
    float mediana;
    float dp;
} Cidade;

typedef struct {
    Cidade *cidades;
    int num_cidades;
    int maior_nota;
    int menor_nota;
    float media;
    float mediana;
    float dp;
} Regiao;


float EX2l (int *notas, int num_alunos);
float media (int *notas, int num_alunos);
float desvio_padrao (int *notas, int num_alunos);
// float mediana(int *alunos_notas, int num_alunos);
void counting_sort(int *notas, int num_alunos);
void soma_counters (int *counter_regiao, int* counter_cidade);
int * count_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos);
int maior(int* count_notas);
int menor(int* count_notas);
double media_counts(int* count_notas, int num_alunos, int multiplier);
float mediana(int* count_notas, int num_alunos, int multiplier);
float DP_counts(int* count_notas, int num_alunos, int multiplier);
void registra_cidade(Cidade *cidade, int* count_notas, int num_alunos);
void registra_regiao(Regiao *regiao, int* count_notas, int num_alunos);
void destroy_regiao(Regiao *regiao);