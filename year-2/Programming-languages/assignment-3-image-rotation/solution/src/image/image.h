#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

#include <stdbool.h>
#include <stdint.h>

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel *data;
};

struct pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

bool image_malloc_data(struct image *img);

void image_destroy(struct image *img);

#endif //LAB3_IMAGE_H
