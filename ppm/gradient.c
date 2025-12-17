#include "../image.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    Image *img = image_create(200, 100);

    if (img == NULL) {
        return 1;
    }

    // Génération du dégradé de bleu
    float step = 255.0 / 200.0;
	float blue = 0;

    for (int x = 0 ; x < img -> width ; x++) {
		blue += step;
        unsigned char rounded_blue = (int) blue;
    	for (int y = 0 ; y < img -> height ; y++) {
        	image_set_pixel(img, x, y, 0, 0, blue);
		}
    }

    image_save_txt(img, "gradient.ppm");

    image_free(img);
    return 0;
}
