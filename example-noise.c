#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "bmp.h"

#define  WIDTH 5000
#define HEIGHT 3000

#define LIMIT 33333

unsigned char random_byte(void) {
    return (unsigned char)(rand() % 256);
}

int main(int argc, char **argv) {
    srand((unsigned)time(NULL));

    unsigned int width = WIDTH;
    unsigned int height = HEIGHT;

    if (argc == 3) {
        width = atoi(argv[1]);
        height = atoi(argv[2]);

        if (width > LIMIT || height > LIMIT) {
            fprintf(
                stderr,
                "width x height too large or invalid: %s %s\n",
                argv[1], argv[2]
            );
            return 1;
        }
    }

    image_t *img = bmp_image_ctor(width, height);
    if (img == NULL) {
        return 1;
    }

    for (unsigned int x = 0; x < width; x++) {
        for (unsigned int y = 0; y < height; y++) {
            color_t color = {
                .red = random_byte(),
                .green = random_byte(),
                .blue = random_byte()
            };
            pixel(x, y, img) = color;
        }
    }

    int rc = bmp_save("noise.bmp", img);
    bmp_image_dtor(img);
    return rc;
}
