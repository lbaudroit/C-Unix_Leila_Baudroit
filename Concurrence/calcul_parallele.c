#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define TAILLE_DONNEES 10
#define MULTIPLICATEUR 1000

static const int DONNEES[TAILLE_DONNEES] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};


typedef struct {
    int valeur;
} args_t;

void* thread_func(void* _args) {
    args_t *arg = _args;
    int valeur = (*arg).valeur;

    // Calcul intensif simulé par des opérations mathématiques
    int64_t resultat = 0;
    for (int j = 0; j < MULTIPLICATEUR; j++) {
        resultat += valeur * valeur + valeur;
        usleep(20);
    }

    printf("Traitement de %d terminé : %ld\n", valeur, resultat);
    pthread_exit(&resultat);
}

int main(void) {
    printf("=== Calcul Parallèle ===\n");

    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);

    /**/
    // TODO: Créer un tableau de pthread_t
    pthread_t threads[TAILLE_DONNEES];

    // TODO: Créer un tableau de structures d'arguments
    args_t args[TAILLE_DONNEES];

    // TODO: Créer et démarrer tous les threads
    for (int i = 0 ; i < TAILLE_DONNEES ; i++) {
        args[i].valeur = DONNEES[i];
        pthread_create(&threads[i], NULL, thread_func, &args[i]);
    }

    // TODO: Attendre tous les threads
    // TODO: Agréger les résultats
    int64_t somme_total = 0;
    for (int i = 0 ; i < TAILLE_DONNEES ; i++) {
        int64_t *result = malloc(sizeof(int64_t));
        pthread_join(threads[i], (void **)&result);
        somme_total += *result;
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    long duree = (fin.tv_sec - debut.tv_sec) * 1000 +
                 (fin.tv_nsec - debut.tv_nsec) / 1000000;

    printf("Résultat total : %ld\n", somme_total);
    printf("Durée : %ld ms\n", duree);

    return 0;
}