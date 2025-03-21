#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define N 5

int globale = 1;

void attendre_aleatoire() {
    int duree = rand() % 100000;
    usleep(duree);
}

int main() {
    pid_t pid;
    srand(time(NULL));

    printf("[Processus: Principal] PID=%d | PPID=%d | globale=%d\n", getpid(), getppid(), globale);

    pid = fork();

    if (pid < 0) {
        perror("Erreur fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Processus fils
        printf("[Processus: Fils] PID=%d | PPID=%d | globale=%d\n", getpid(), getppid(), globale);
        for (int i = 0; i < N; i++) {
            attendre_aleatoire();
            globale *= 2;
            printf("[Processus: Fils] i=%d | globale=%d\n", i + 1, globale);
        }
        printf("*** FIN du processus < %d > | globale = %d ***\n", getpid(), globale);
        exit(EXIT_SUCCESS);
    } else { // Processus père
        printf("[Processus: Père] PID=%d | PPID=%d | globale=%d\n", getpid(), getppid(), globale);
        for (int i = 0; i < N; i++) {
            attendre_aleatoire();
            globale += 10;
            printf("[Processus: Père] i=%d | globale=%d\n", i + 1, globale);
        }
        wait(NULL); // Attente de la fin du processus fils
        printf("*** FIN du processus < %d > | globale = %d ***\n", getpid(), globale);
    }
    return 0;
}