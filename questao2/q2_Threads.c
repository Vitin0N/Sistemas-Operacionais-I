/*
    Programa criado para a resolução da atividade de 
    Sistema Operacional I

    Nome do arquivo: q2_Threads.c

    Equipe:
        (Nome | Matrícula)
        - Victor Gabriel da Silva Menezes | 20240008323
        - Luiz Henrique Santos da Graça   | 20240008261
    
        Objetivo:
            Este programa utiliza a biblioteca pthread para dividir 
            a tarefa em 3 fluxos de execução (threads) que rodam 
            simultaneamente dentro do mesmo processo, compartilhando 
            a mesma memória. O objetivo é verificar o tempo de criação 
            e execução das threads e comparar se essa abordagem é mais 
            eficiente que a versão sequencial.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>    // Biblioteca para usar conceitos de Threads (pthread_t, pthread_create, pthread_join)
#include <math.h>       // Funções matemáticas
#include <time.h>       // Para uso de funções relacionado a tempo(clock, time(NULL))

#define TAMANHO 10000

// Variáveis globais
int ar[TAMANHO];
double media, mediana, desvioPadrao;

// Função de comparação do Qsort
int compara(const void* a, const void* b){
    return (*(int*)a - *(int*) b);
}

// Calculos de dados estátisticos do array
void *calc_Media(void *arg);
void *calc_Mediana(void *arg);
void *calc_DesvioPadrao(void *arg);


int main(){

    // Criação das Threads
    pthread_t t1, t2, t3;
    srand(time(NULL));

    for(int i = 0; i < TAMANHO; i++) {ar[i] = rand() % 101;} // Randomiza 10.000 valores para o array

    printf("--- [Threads] Iniciando Processamento --- \n");

    clock_t inicio = clock(); // Marca o tempo do inicio do processo

    // Criação das Threads
    pthread_create(&t1, NULL, calc_Media, NULL);        // Define thread para calculo de média
    pthread_create(&t2, NULL, calc_Mediana, NULL);      // Define thread para calculo de mediana
    pthread_create(&t3, NULL, calc_DesvioPadrao, NULL); // Define thread para calculo de desvio padrão

    // Espera as threads terminarem sua execução
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);

    clock_t fim = clock(); // Marca o tempo do fim do processo

    // Calculo do tempo que demorou a execução do programa (convertido em segundos)
    double tempo_final = ((double)(fim - inicio)) / CLOCKS_PER_SEC;

    printf("Media: %.2f\n", media);
    printf("Mediana: %.2f\n", mediana);
    printf("Desvio Padrao: %.2f\n", desvioPadrao);
    printf("Tempo de execucao (Threads): %lf(s)\n", tempo_final);

    return 0;
}

void *calc_Media(void *arg){
    double soma = 0;
    for(int i = 0; i < TAMANHO; i++) { soma += ar[i]; } // Percorre todo o array somando cada elemento
    media = soma / TAMANHO; // Divide pelo tamanho do array

    pthread_exit(NULL); // Finaliza a execução da thread
}

void *calc_Mediana(void *arg){
    int *copia = malloc(TAMANHO * sizeof(int));
    for(int i = 0; i < TAMANHO; i++) { copia[i] = ar[i]; }
    qsort(copia, TAMANHO, sizeof(int), compara); // Deixa a copia do array ordenado

    mediana = (copia[TAMANHO/2] + copia[TAMANHO/2 - 1]) / 2.0;

    free(copia); // Libera a copia do array
    pthread_exit(NULL); // Finaliza a execução da thread
}

void *calc_DesvioPadrao(void *arg){
    double soma = 0, media_local, dif_quadrado = 0;

    for (int i = 0; i < TAMANHO; i++) { soma += ar[i]; }
    media_local = soma / TAMANHO;

    for(int i = 0; i < TAMANHO; i++) {dif_quadrado += pow(ar[i] - media_local, 2); }

    desvioPadrao = sqrt(dif_quadrado / TAMANHO);
    pthread_exit(NULL); // Finaliza a execução da thread
}