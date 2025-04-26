#include "texture.h"
#include "lodepng.h"
#include <stdio.h>
#include <stdlib.h>

void load_texture(texture_t *texture, const char* path) {
    unsigned char error;

    error = lodepng_decode32_file(&texture->pixels, &texture->width, &texture->height, path);
    if (error) {
        printf("error %d\n", error);
        exit(EXIT_FAILURE);
    }
}

void free_texture(texture_t *texture) {
    free(texture->pixels);
}