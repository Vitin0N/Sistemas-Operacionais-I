/*
    Programa criado para a resolução da atividade de 
    Sistema Operacional I

    Nome do arquivo: q2_processos.c

    Equipe:
        (Nome | Matrícula)
        - Victor Gabriel da Silva Menezes | 20240008323
        - Luiz Henrique Santos da Graça   | 20240008261
    
        Objetivo:
            Este programa utiliza a chamada de sistema fork() 
            para dividir a tarefa criando 3 cópias independentes 
            do programa (processos filhos). Como a memória é separada, 
            ele utiliza pipes para comunicar os resultados de volta ao 
            processo pai. O objetivo é medir o 'custo' (overhead) de 
            criar processos inteiros e comparar o desempenho com a versão 
            de threads e a sequencial.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>         // Chamada do sistema UNIX (getpid, fork, execl)
#include <sys/wait.h>       // Serve para o controle de espera (wait)
#include <sys/types.h>      // Tipos de dados (pid_t)
#include <math.h>
#include <time.h>

#define TAMANHO 10000

// Variável global
int arr[TAMANHO];

// Função de comparação para o qsort
int compara(const void* a, const void* b){
    return (*(int*)a - *(int*)b);
}

// Calculos de dados estátisticos do array
double calcMedia();
double calcMediana();
double calcDesvioPadrao();


int main(){
    
    // Criação de pipes ([0] -> leitura | [1] -> escrita)
    int pipe_media[2], pipe_mediana[2], pipe_desvioPadrao[2];
    pid_t pid;

    srand(time(NULL));
    for(int i = 0; i < TAMANHO ; i++) { arr[i] = rand() % 101; } // Preenche o array

    // Define como Pipes
    pipe(pipe_media);
    pipe(pipe_mediana);
    pipe(pipe_desvioPadrao);

    printf("--- Inicializando Processos filhos --- \n");
    clock_t inicio = clock(); // Define o inicio da execução

    if(fork() == 0){
        close(pipe_media[0]); // Fecha a leitura

        double resultado = calcMedia(); // Recebe a media do array
        write(pipe_media[1], &resultado, sizeof(double)); // Envia a informação para o "Pai" via pipe

        close(pipe_media[1]); // Fecha a escrita
        exit(0);    // Encerra o processo
    }

    if(fork() == 0){
        close(pipe_mediana[0]); // Fecha a leitura

        double resultado = calcMediana();   // Recebe a mediana do array
        write(pipe_mediana[1], &resultado, sizeof(double)); // Envia a informação para o "Pai" via pipe

        close(pipe_mediana[1]); // Fecha a escrita
        exit(0); // Encerra o processo
    }

        if(fork() == 0){
        close(pipe_desvioPadrao[0]); // Fecha a leitura

        double resultado = calcDesvioPadrao();   // Recebe o desvio padrão do array
        write(pipe_desvioPadrao[1], &resultado, sizeof(double)); // Envia a informação para o "Pai" via pipe

        close(pipe_desvioPadrao[1]); // Fecha a escrita
        exit(0);  // Encerra o processo
    }

    close(pipe_media[1]);
    close(pipe_mediana[1]);
    close(pipe_desvioPadrao[1]);

    double r1, r2, r3;

    // Processo pai lê os resultados dos pipes
    read(pipe_media[0], &r1, sizeof(double));
    read(pipe_mediana[0], &r2, sizeof(double));
    read(pipe_desvioPadrao[0], &r3, sizeof(double));

    // Espera os filhos terminarem
    wait(NULL);
    wait(NULL);
    wait(NULL);

    clock_t fim = clock();  // Define o fim da execução
    double tempo_execucao = ((double)(fim - inicio)) / CLOCKS_PER_SEC; // Traduz para segundos

    printf("Media: %.2f\n", r1);
    printf("Mediana: %.2f\n", r2);
    printf("Desvio Padrao: %.2f\n", r3);
    printf("Tempo de Execucao(Processos): %lf(s)\n", tempo_execucao);

    return 0;
}

double calcMedia(){ // Calculo de Media
    int soma = 0;

    for(int i = 0; i < TAMANHO; i++){ soma += arr[i]; }

    return (soma / TAMANHO) * 1.0;
}

double calcMediana(){ // Calculo de Mediana
    int *copia = malloc(TAMANHO * sizeof(int));
    double mediana;
    for(int i = 0 ; i < TAMANHO ; i++) { copia[i] = arr[i]; }

    qsort(copia, TAMANHO, sizeof(int), compara);

    mediana = (copia[TAMANHO/2] + arr[TAMANHO/2 - 1]) / 2.0;
    free(copia);

    return mediana;
}

double calcDesvioPadrao(){ // Calculo do Desvio Padrão
    double media = calcMedia();
    double dif_quadrado = 0;

    for(int i = 0; i<TAMANHO; i++){
        dif_quadrado += pow(arr[i] - media, 2);
    }

    return sqrt(dif_quadrado / TAMANHO);
}