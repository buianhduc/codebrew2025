#ifndef _TEXTURE_H
#define _TEXTURE_H

typedef struct {
    unsigned int width, height;
    unsigned char *pixels;
} texture_t;

void load_texture(texture_t* texture, const char* path);
void free_texture(texture_t* texture);

#endif