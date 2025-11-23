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
	if (img != NULL && x >= 0 && x < img -> width && y >= 0 && y < img -> height) {
        int index = ( y * img -> width + x ) * 3;
        img -> pixels[index + 0] = r;
        img -> pixels[index + 1] = g;
        img -> pixels[index + 2] = b;
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

unsigned char image_get_red(Image *img, int x, int y) {
	int index = ( y * img -> width + x ) * 3;
	return img -> pixels[index];
}

unsigned char image_get_green(Image *img, int x, int y) {
	int index = ( y * img -> width + x ) * 3;
	return img -> pixels[index + 1];
}

unsigned char image_get_blue(Image *img, int x, int y) {
	int index = ( y * img -> width + x ) * 3;
	return img -> pixels[index + 2];

}

Image* image_read_txt(const char *filename) {
	FILE *fp = fopen(filename, "r");
    Image *img = 0;

    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
    }

	int line_no = 1;
    int was_read = 0;
    int y = 0;
    int width, height;

	do {
        int buffer_size = 0;
	    char *line = NULL;
		was_read = getline(&line, &buffer_size, fp);

        /* Création de l'image avec sa longueur et largeur */
        if (line_no == 2) {
            sscanf(line, "%d %d", &width, &height);
            img = image_create(width, height);
        }

        /* Valeurs des pixels */
        if (line_no > 3) {
            int x = 0;
            unsigned char value, r, g, b = 0;

            int offset = 0;
            char* char_ptr = line;

            while (sscanf(char_ptr, "%hhd %hhd %hhd %n", &r, &g, &b, &offset) == 3) {
                image_set_pixel(img, x, y, r, g, b);
                char_ptr += offset;
                x++;
            }
            y++;
        }

		line_no++;
        free(line);
	} while (was_read != -1);

    fclose(fp);

    return img;
}

void image_save_bin(Image *img, const char *filename) {
    if (img == NULL) return;

    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
        return;
    }

    fprintf(fp, "P6\n");

    // Écriture des dimensions
    fprintf(fp, "%d %d\n", img -> width, img -> height);

    // Écriture de la valeur maximale
    fprintf(fp, "255\n");

    // Ecriture des pixels (sans retour à la ligne)
    fprintf(fp, img -> pixels);

    fclose(fp); // Fermeture du fichier
    printf("Image PPM binaire créée avec succès !\n");
}

Image* image_read_bin(const char *filename) {
    FILE *fp = fopen(filename, "r");
    Image *img = 0;

    if (fp == NULL) {
        perror("Erreur d'ouverture du fichier");
    }

    int line_no = 1;
    int was_read = 0;
    int y = 0;
    int width, height;

    do {
        int buffer_size = 0;
        char *line = NULL;
        was_read = getline(&line, &buffer_size, fp);
        printf("line : %s\n", line);

        /* Création de l'image avec sa longueur et largeur */
        if (line_no == 2) {
            sscanf(line, "%d %d", &width, &height);
            img = image_create(width, height);
        }

        /* Valeurs des pixels */
        if (line_no > 3) {
            img -> pixels = line;
        }

        line_no++;
        free(line);
    } while (was_read != -1);

    fclose(fp);

    return img;
}