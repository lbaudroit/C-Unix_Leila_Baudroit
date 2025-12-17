#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct Arg {
    int increment;
} Arg;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static int compteur_global = 0;

void reset_compteur(void) {
    compteur_global = 0;
}

int get_compteur(void) {
    return compteur_global;
}

void* incrementeur_thread(void* arg) {
    struct Arg* args_value = arg;
    for (int i = 0; i < args_value -> increment ; i++) {
        // voir commentaire compteur_dangereux pour explication problème
        pthread_mutex_lock(&mutex);
        // ici on fait lecture + écriture, donc on protège les deux opérations
        compteur_global++;
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

int executer_test(int nb_threads, int nb_increments) {
    reset_compteur();

    pthread_t* threads = malloc(nb_threads * sizeof(pthread_t));

    // Instancier un tableau de structure pour les arguments.
    struct Arg * args = malloc(nb_threads * sizeof(Arg));

    // Création des threads
    for (int i = 0; i < nb_threads; i++) {
        args[i].increment = nb_increments;
        pthread_create(&threads[i], NULL, incrementeur_thread, &args[i]);
    }

    for (int i = 0; i < nb_threads; i++)
        pthread_join(threads[i], NULL);

    int resultat = get_compteur();

    // Qu'est ce qui a été oublié ici ?
    free(threads);
    free(args);

    return resultat;
}

int main(void) {
    const int NB_TESTS = 50;
    const int NB_THREADS = 10;
    const int NB_INCREMENTS = 100000;
    const int ATTENDU = NB_THREADS * NB_INCREMENTS;

    printf("=== Test Race Condition ===\n");
    printf("Configuration : %d threads × %d incréments\n", NB_THREADS, NB_INCREMENTS);
    printf("Valeur attendue : %d\n\n", ATTENDU);

    int nb_corrects = 0;
    int min = ATTENDU;
    int max = 0;

    for (int test = 0; test < NB_TESTS; test++) {
        int resultat = executer_test(NB_THREADS, NB_INCREMENTS);

        if (resultat == ATTENDU) {
            nb_corrects++;
            printf("Test %2d : %d v \n", test + 1, resultat);
        } else {
            printf("Test %2d : %d x (différence: %d)\n", test + 1, resultat, ATTENDU - resultat);
        }

        if (resultat < min) min = resultat;
        if (resultat > max) max = resultat;
    }

    printf("\n=== Statistiques ===\n");
    printf("Tests corrects : %d/%d (%.1f%%)\n",
           nb_corrects, NB_TESTS, (100.0 * nb_corrects) / NB_TESTS);
    printf("Valeur min : %d (perte de %d)\n", min, ATTENDU - min);
    printf("Valeur max : %d\n", max);

    return 0;
}