#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//#define LIMITE_HAUTE 1000 // <- Décommentre cette ligne pour version fixe

int main() {
    int pipe_fils_to_pere[2];
    int pipe_pere_to_fils[2];
    pid_t pid;
    int secret, guess, response;

    int low = 1;
    int high;

    // === Version INTERACTIVE ===
    // L'utilisateur choisit la borne haute
    printf("Entrez la borne haute pour le Juste Prix : ");
    scanf("%d", &high);

    // === Version FIXE ===
    // Pour avoir une borne haute prédéfinie, commenter les deux lignes ci-dessus (19 et 20) de === Version INTERACTIVE ===
    // et décommenter la ligne 6 : define LIMITE_HAUTE et la ligne suivante :
    // high = LIMITE_HAUTE;

    // Création des deux tubes
    if (pipe(pipe_fils_to_pere) == -1 || pipe(pipe_pere_to_fils) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid > 0) { // Processus père
        close(pipe_fils_to_pere[1]); // Ferme l'écriture du pipe 1
        close(pipe_pere_to_fils[0]); // Ferme la lecture du pipe 2

        srand(time(NULL));
        secret = rand() % (high - low + 1) + low;
        printf("[PÈRE] Nombre secret généré entre %d et %d (non affiché au fils)\n", low, high);

        while (1) {
            read(pipe_fils_to_pere[0], &guess, sizeof(int));
            printf("[PÈRE] Le fils propose : %d\n", guess);

            if (guess < secret) {
                response = -1; // Trop bas
            } else if (guess > secret) {
                response = 1; // Trop haut
            } else {
                response = 0; // Gagné
                printf("[PÈRE] Le fils a trouvé le bon nombre ! (%d)\n", secret);
            }

            write(pipe_pere_to_fils[1], &response, sizeof(int));

            if (response == 0) break;
        }

        close(pipe_fils_to_pere[0]);
        close(pipe_pere_to_fils[1]);
    } else { // Processus fils
        close(pipe_fils_to_pere[0]); // Ferme la lecture du pipe 1
        close(pipe_pere_to_fils[1]); // Ferme l'écriture du pipe 2

        while (1) {
            guess = (low + high) / 2;
            printf("[FILS] Je propose : %d\n", guess);
            write(pipe_fils_to_pere[1], &guess, sizeof(int));
            read(pipe_pere_to_fils[0], &response, sizeof(int));

            if (response == 0) {
                printf("[FILS] J'ai trouvé le nombre ! (%d)\n", guess);
                break;
            } else if (response == -1) {
                printf("[FILS] Trop bas !\n");
                low = guess + 1;
            } else {
                printf("[FILS] Trop haut !\n");
                high = guess - 1;
            }
        }

        close(pipe_fils_to_pere[1]);
        close(pipe_pere_to_fils[0]);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
