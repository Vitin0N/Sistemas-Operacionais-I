/*
    Programa criado para a resolução da atividade de 
    Sistema Operacional I

    Nome do arquivo: questao1resposta.c

    Equipe:
        (Nome | Matrícula)
        - Victor Gabriel da Silva Menezes | 20240008323
        - Luiz Henrique Santos da Graça   | 20240008261
    
    Este programa foi feito para mostrar a hierarquia de processos usando chamadas fork().
    Criando nesse exmplo Processo Pai -> Processos Filhos -> Processos Neto.
    Sendo o processo neto responsável por executar um comando linux da nossa escolha.

    Objetivo:
        O objetivo foi ver como os processos se comportam. Nesse caso:
        - O processo pai (P1) cria os processos filhos (F1 e F2)
        - Cada filho cria um neto do processo pai e cada neto executa um comando
            -F1 -> N1('ls')     e N2('date')
            -F2 -> N3('whoami') e N4('pwd')
        - Os filhos encerram quando os netos terminarem sua execução
        - O pai encerra quando os filhos encerrarem sua execução
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     // Chamada do sistema UNIX (getpid, fork, execl)
#include <sys/wait.h>   // Serve para o controle de espera (wait)
#include <sys/types.h>  // Tipos de dados (pid_t)

// Função para criação de nós filhos 'Fn' de P1, 
// recebe apenas o nome como parâmentro
void criaNo_Filho(char* name);

// Função para criar netos 'Ni' de P1, 
// recebe nome, caminho e comando linux para executar
void criaNo_Neto(char* name, char* path_cmd, char* cmd);

int main() {
    // Mostra o PID do processo pai e 
    // que ele foi executado antes dos filhos e netos
    printf("[P1] Processo pai (PID: %d).\n", getpid());

    criaNo_Filho("F1"); // Cria o filho 'F1'
    criaNo_Filho("F2"); // Cria o filho 'F2'

    // O processo P1 espera os processos
    // F1 e seus netos N1 e N2 terminarem, além de 
    // F2 e seus netos N3 e N4 terminarem para que ele possa seguir com o resto do código
    wait(NULL); 
    wait(NULL);
    
    // Mostra mensagem que os processos filhos e netos ja foram concluidos
    printf("[P1] Todos os filhos foram executados. Fim do programa!\n"); 

    return 0;
}

void criaNo_Filho(char* name){
    pid_t pid = fork(); // Cria um novo processo Filho 'Fn'

    if(pid == 0){
        // Mostra o PID do atual processo (F1 ou F2) e o PID de P1 (processo pai)
        printf("[%s] Iniciando (PID %d) | Pai: P1 (PID %d)\n", name, getpid(), getppid());

        // Criação de processos netos do processo P1
        if(name[1] == '1'){  

            // Criação de processos filhos de 'F1'
            criaNo_Neto("N1", "/bin/ls", "ls"); // Lista todos os arquivos e pasta dentro da pasta

            criaNo_Neto("N2", "/bin/date", "date"); // Consulta data e hora atual completa

        } else if(name[1] == '2'){

            // Criação de processos filhos de 'F2'
            criaNo_Neto("N3", "/usr/bin/whoami", "whoami"); //Imprime o user_name que está executando o processo+-

            criaNo_Neto("N4", "/bin/pwd", "pwd"); // Mostra em qual pasta eu o processo esta sendo executado
        }

        // Processo 'Fn' esperam os processos filhos terminarem
        wait(NULL);
        wait(NULL);

        // Os processos netos de P1 terminaram, encerra os processos filhos
        printf("[%s] meus filhos terminaram. Encerrando [%s].\n", name, name);
        exit(0);
    }
}

void criaNo_Neto(char* name, char* path_cmd, char* cmd){
    pid_t pid = fork(); // Cria processo 'Nn', neto de P1
    
    if(pid < 0){
        perror("Erro no fork!"); // Caso pid retorne valor negativo então houve erro na criação de processo
        exit(1);
    }

    if(pid == 0){
        // Nome do processo neto está executando juntamente com seu PID
        printf("[%s] (PID: %d) - (PID do pai: %d) - executando '%s'...\n", name, getpid(), getppid(), cmd);

        // Substitui o processo pelo comando linux
        execl(path_cmd, cmd, (char*)NULL); // Executa o código passado pelo o processos 'Fn'

        // Erro do execl
        perror("Erro ao executar execl!"); // Só executa caso o 'execl' der erro
        exit(1);
    }
    // Encerra processo neto de P1
}