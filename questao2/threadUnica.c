/*
    Programa criado para a resolução da atividade de 
    Sistema Operacional I

    Nome do arquivo: threadUnica.c

    Equipe:
        (Nome | Matrícula)
        - Victor Gabriel da Silva Menezes | 20240008323
        - Luiz Henrique Santos da Graça   | 20240008261
    
    Este programa foi feito para mostrar como funcionaria uma execução
    para calcular media, mediana e desvio padrão de um array de 10.000
    elementos. Mostrando em quanto tempo essa execução, de um programa
    sequencial, demora em relação a Threads e Processos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>       // Funções matemáticas
#include <time.h>       // Para uso de funções relacionado a tempo(clock, time(NULL))

#define TAMANHO 10000

// Variáveis globais
int arr[TAMANHO];
double media, mediana, desvioPadrao; 

// Função para comparar no Qsort
int comparar(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Calculos de dados estátisticos do array
void calcMedia();
void calcMediana();
void calcDesvioPadrao();

int main(){
    srand(time(NULL));
    for(int i = 0 ; i < TAMANHO ; i ++){ arr[i] = rand() % 101; }  // Randomiza 10.000 valores para o array

    printf("--- [Thread Unica] Inicializando processamento ---\n");

    clock_t inicio = clock(); // Marca o tempo do inicio do processo

    calcMedia();            // Primeiro calcula a média
    calcMediana();          // Segundo calcula a mediana
    calcDesvioPadrao();     // Terceiro calcula o desvio padrão

    clock_t fim = clock(); // Marca o tempo do fim do processo

    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Média: %.2f\n", media);
    printf("Mediana: %.2f\n", mediana);
    printf("Desvio Padrão: %.2f\n", desvioPadrao);
    printf("Tempo total (Thread Unica): %lf(s)\n", tempo_execucao);

    return 0;
}

void calcMedia(){
    int soma = 0;

    for(int i = 0; i < TAMANHO ; i++){ soma += arr[i]; }

    media = soma / TAMANHO; // Guarda na variável global
}

void calcMediana(){
    int *copia = malloc(TAMANHO * sizeof(int));
    for(int i = 0 ; i < TAMANHO ; i++) { copia[i] = arr[i]; }

    qsort(copia, TAMANHO, sizeof(int), comparar);
    mediana = (copia[TAMANHO/2] + copia[TAMANHO/2 - 1]) / 2.0; // Guarda na variável global

    free(copia); // Libera a copia do array
}

void calcDesvioPadrao(){
    double media_local = media, dif_quadrado = 0;
    media_local = media;

    for(int i = 0 ; i < TAMANHO ; i++) { dif_quadrado += pow(arr[i] - media, 2);}

    desvioPadrao = sqrt(dif_quadrado / TAMANHO); // Guarda na variável global
}

