#include "image.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Image *img = image_create(200, 1);

    if (img == NULL) {
        return 1;
    }

    // Génération du dégradé de bleu
    float step = 255.0 / 200.0;

    for (float blue = 0, x = 0 ; x < img -> width ; x++, blue += step) {
        unsigned char rounded_blue = (int) blue;
        image_set_pixel(img, x, 1, 0, 0, blue);
    }

    image_save_txt(img, "gradient.ppm");

    image_free(img);
    return 0;
}
