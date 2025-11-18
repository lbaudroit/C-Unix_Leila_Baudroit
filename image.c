#include "image.h"
#include <stdlib.h>
#include <string.h>

Image* image_create(int width, int height) {
    Image *img = (Image *)malloc(sizeof(Image));
    img -> width = width;
    img -> height = height;

    int expected_size = width * height * 3 * sizeof(unsigned char);
    unsigned char *pixels = malloc(expected_size);
    img -> pixels = (unsigned char*) pixels;
    return img;
}

void image_free(Image *img) {
    free(img -> pixels);
    free(img);
}

void image_set_pixel(Image *img, int x, int y, unsigned char r, unsigned char g, unsigned char b) {
    if (img != NULL && x >= 0 && x < img->width && y >= 0 && y < img->height) {
        int index = ( y * img -> width + x ) * 3;
        img->pixels[index + 0] = r;
        img->pixels[index + 1] = g;
        img->pixels[index + 2] = b;
    }
}

void image_save_txt(Image *img, const char *filename) {
    if (img == NULL) return;

    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    fprintf(fp, "P3\n");

    // Écriture des dimensions
    fprintf(fp, "%d %d\n", img -> width, img -> height);

    // Écriture de la valeur maximale
    fprintf(fp, "255\n");

    for (int y = 0 ; y < img -> height ; y++) {
        for (int x = 0 ; x < img -> width ; x++) {
            int index = ( y * img -> width + x ) * 3;
            fprintf(fp, "%d %d %d ", img -> pixels[index], img -> pixels[index + 1], img -> pixels[index + 2]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp); // Fermeture du fichier
    printf("Image PPM créée avec succès !\n");
}
