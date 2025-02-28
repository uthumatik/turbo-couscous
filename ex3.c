#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    pid_t pid;
    srand(time(NULL));
    
    pid = fork();
    
    if (pid == 0) {
        int sleep_time = rand() % 5 + 1;
        printf("[fils] PID: %d, PPID: %d\n", getpid(), getppid());
        printf("[fils] Je m'endors pour %ds...\n", sleep_time);
        sleep(sleep_time);
        printf("[fils] Je suis réveillé !\n");
        printf("*** FIN du processus fils (%d) ***\n", getpid());
        exit(0);
    } else {
        printf("[père] PID: %d, PPID: %d\n", getpid(), getppid());
        printf("[père] En attente du réveil de son fils...\n");
        wait(NULL);
        printf("[père] Mon fils s'est réveillé !\n");
        printf("*** FIN du processus père (%d) ***\n", getpid());
    }
    return 0;
}