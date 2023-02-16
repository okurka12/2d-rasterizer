#include "rasterizer.h"
#include <stdio.h>


int main() {

    /* this is only a test obviously main.c is not going to look like this */
    color_t red = {.red = 255};
    color_t yel = {.red = 255, .green = 255};
    color_t cyan = {.green = 255, .blue = 255};
    char width = 100;
    char height = 50;
    
    color_t picture[height * width];
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x < width / 2 || y < height / 2) {
                picture[width * y + x] = yel;
            }
            else {
                picture[width * y + x] = red;
            }
        }
    }
    create_bmp("obrazecek_prvni.bmp", picture, width, height);

    return 0;
}