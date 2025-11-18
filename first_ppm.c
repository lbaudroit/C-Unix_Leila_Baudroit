#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("firstPPM.ppm", "w");

    if (fp == NULL) {
        perror("Erreur d'ouvertur du fichier");
        return EXIT_FAILURE;
    }

    fprintf(fp, "P3\n");

    // Écriture des dimensions
    fprintf(fp, "3 2\n");

    // Écriture de la valeur maximale
    fprintf(fp, "255\n");

    // Écriture des pixels
    // Première ligne : rouge, vert, bleu
    fprintf(fp, "255 0 0 0 255 0 0 0 255\n");

    // Deuxième ligne : jaune, blanc, noir
    fprintf(fp, "255 255 0 255 255 255 0 0 0\n");

    fclose(fp); // Fermeture du fichier
    printf("Image PPM créée avec succès !\n");

    return EXIT_SUCCESS;
}