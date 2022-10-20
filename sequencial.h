#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>
#define MAX_NOTA 100


float media (int *notas, int num_alunos);
float desvio_padrao (int *notas, int num_alunos);
float mediana(int *notas, int *alunos_notas, int num_alunos);
void counting_sort(int *notas, int num_alunos);
void soma_counters (int *counter_regiao, int* counter_cidade);
int * ordenar_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos);
int maior(int* count_notas);
int menor(int* count_notas);
double media_counts(int* count_notas, int num_alunos, int multiplier);
float mediana_counts(int* count_notas, int num_alunos, int multiplier);
float DP_counts(int* count_notas, int num_alunos, int multiplier);
