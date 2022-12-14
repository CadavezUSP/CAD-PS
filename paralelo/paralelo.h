#ifndef _PARALELO_H_
#define _PARALELO_H_

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<time.h>
#include <omp.h>
#define MAX_NOTA 100
#define NUM_THREADS 8

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

typedef struct {
    int regiao;
    int cidade;
    float maxMedia;
} MyMax;

float EX2l(int *count_notas, int num_alunos);
float media (int *count_notas, int num_alunos);
void counting_sort(int *notas, int num_alunos);
void soma_counters (int *counter_regiao, int* counter_cidade);
int * count_notas(int ***matriz_regioes, int regiao, int cidade, int num_alunos);
int maior(int* count_notas);
int menor(int* count_notas);
double media_counts(int* count_notas, int num_alunos, int multiplier);
float mediana(int* count_notas, int num_alunos, int multiplier);
void destroy_regiao(Regiao *regiao);
void imprime_valores(Regiao *regioes, int r);
void maxCidade(int *omp_out, int *omp_in);


#endif