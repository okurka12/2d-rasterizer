#include <stdlib.h>
#include <time.h>
#include "bmp.h"

#define  WIDTH 5000
#define HEIGHT 3000

unsigned char random_byte(void) {
    return (unsigned char)(rand() % 256);
}

int main(void) {
    srand((unsigned)time(NULL));

    image_t *img = bmp_image_ctor(WIDTH, HEIGHT);
    if (img == NULL) {
        return 1;
    }

    for (unsigned int x = 0; x < WIDTH; x++) {
        for (unsigned int y = 0; y < HEIGHT; y++) {
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
