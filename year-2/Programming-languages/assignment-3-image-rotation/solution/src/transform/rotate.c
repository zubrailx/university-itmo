#include "rotate.h"

#include <malloc.h>

struct image rotate( struct image const source){
    uint64_t width = source.width;
    uint64_t height = source.height;
    struct pixel *data = source.data;

    struct pixel *temp_arr = malloc(sizeof(struct pixel) * height * width);
//    if (!temp_arr) { return NULL; }

    for (uint64_t i = 0; i < height; ++i) {
        for (uint64_t j = 0; j < width; ++j) {
            *(temp_arr + j * height + (height - 1 - i)) = *(data + i * width + j);
        }
    }
    struct image img = {0};
    img.width = height;
    img.height = width;
    img.data = temp_arr;
    return img;
}

