#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <math.h>


#define TAILLE_BUFFER 5

struct Args
{
    int id;
    int nb_produits;
} Args;

// Buffer circulaire
static int buffer[TAILLE_BUFFER];
static int index_prod = 0;
static int index_cons = 0;


// TODO: Initialisé les 3 sémaphores ci-dessous avec les bonnes valeurs initiales via sem_init
// Le deuxième paramètre `0` signifie "partagé entre threads du même processus" (vs `1` pour inter-processus).

static sem_t places_libres;
static sem_t elements_disponibles;
static sem_t mutex_buffer; // Qulle est le soucis ici ? Ca devrait être une mutex !!

void ajouter_buffer(int produit)
{
    sem_wait(&mutex_buffer);
    // Quelle soucis peut on imaginer ici ?
    buffer[index_prod] = produit;
    index_prod++; // Circulaire : utilisez la congruence
    if (index_prod == TAILLE_BUFFER)
    {
        index_prod = 0;
    }

    sem_post(&mutex_buffer);
}

int retirer_buffer(void)
{
    sem_wait(&mutex_buffer);
    // Quelle soucis peut on imaginer ici ?
    int produit = buffer[index_cons];
    index_cons--; // Circulaire : utilisez la congruence
    if (index_prod == -1)
    {
        index_prod = TAILLE_BUFFER - 1;
    }
    sem_post(&mutex_buffer);
    return produit;
}

void* thread_producteur(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < 10; i++)
    {
        int produit = id * 100 + i; // Produit unique

        // TODO: 1. Attendre une place libre
        sem_wait(&places_libres);

        // TODO: 2. Ajouter l'élément au buffer
        ajouter_buffer(produit);

        printf("[Producteur %d] Ajouté : %d (index=%d)\n",
               id, produit, (index_prod - 1 + TAILLE_BUFFER) % TAILLE_BUFFER);

        // TODO: 3. Signaler qu'un élément est disponible
        usleep(50);
        sem_post(&places_libres);
    }

    printf("[Producteur %d] terminé (10 produits)\n", id);
    return NULL;
}

void* thread_consommateur(void* arg)
{
    int id = *(int*)arg;

    for (int i = 0; i < 7; i++)
    {
        // TODO: 1. Attendre qu'un élément soit disponible
        sem_wait(&elements_disponibles);

        // TODO: 2. Retirer un élément du buffer
        int produit = retirer_buffer();

        printf("[Consommateur %d] Retiré : %d (index=%d)\n",
               id, produit, (index_cons - 1 + TAILLE_BUFFER) % TAILLE_BUFFER);

        // TODO: 3. Signaler qu'une place est libre
        sem_post(&elements_disponibles);

        usleep(50);
    }

    printf("[Consommateur %d] terminé (7 produits)\n", id);
    return NULL;
}


int main(void)
{
    sem_init(&places_libres, 0, TAILLE_BUFFER);
    sem_init(&elements_disponibles, 0, 0);
    sem_init(&mutex_buffer, 0, 1);

    srand(time(NULL));

    // Initialisation des sémaphores

    printf("=== Producteur-Consommateur ===\n");
    printf("Taille buffer : %d\n\n", TAILLE_BUFFER);

    // Scénario : 2 producteurs × 10 = 20 produits
    //            3 consommateurs × 7 = 21 demandes
    // → 1 consommateur va bloquer indéfiniment !

    int NB_PRODUCTEURS = 2;
    int NB_CONSOMMATEURS = 3;

    pthread_t producteurs[NB_PRODUCTEURS];
    pthread_t consommateurs[NB_CONSOMMATEURS];
    int ids_prod[2] = {1, 2};
    int ids_cons[3] = {1, 2, 3};

    // Démarrage
    for (int i = 0; i < NB_PRODUCTEURS; i++)
    {
        pthread_create(&producteurs[i], NULL, thread_producteur, &ids_prod[i]);
    }
    for (int i = 0; i < NB_CONSOMMATEURS; i++)
    {
        pthread_create(&consommateurs[i], NULL, thread_consommateur, &ids_cons[i]);
    }

    // Attendre 2 secondes
    sleep(2);

    // Annuler les threads bloqués (pthread_cancel)
    int bloques[2] = {0, 0};
    for (int i = 0; i < NB_PRODUCTEURS; i++)
    {
        bloques[0] += pthread_cancel(producteurs[i]);
    }
    for (int i = 0; i < NB_CONSOMMATEURS; i++)
    {
        bloques[1] += pthread_cancel(consommateurs[i]);
    }

    printf("Threads bloqués : %d (%d producteurs, %d consommateurs)\n",
           bloques[0] + bloques[1],
           bloques[0],
           bloques[1]);

    // Nettoyage
    sem_destroy(&places_libres);
    sem_destroy(&elements_disponibles);
    sem_destroy(&mutex_buffer);

    return 0;
}
