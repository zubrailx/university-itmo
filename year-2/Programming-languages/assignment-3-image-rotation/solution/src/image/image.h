#ifndef LAB3_IMAGE_H
#define LAB3_IMAGE_H

#include <stdbool.h>
#include <stdint.h>

struct pixel {
    uint8_t b;
    uint8_t g;
    uint8_t r;
};

struct image {
    uint64_t width;
    uint64_t height;
    struct pixel *data;
};

// optional
struct image_optional {
    bool is_valid;
    struct image image;
};

static const struct image_optional image_none;

struct image_optional image_some(struct image img);

struct image_optional image_create_empty(uint64_t width, uint64_t height);
struct image_optional copy_image(struct image source);
void image_destroy(struct image *img);


#endif //LAB3_IMAGE_H
