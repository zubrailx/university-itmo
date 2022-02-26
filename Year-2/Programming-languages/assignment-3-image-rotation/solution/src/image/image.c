#include "image.h"

#include <malloc.h>


static const struct image_optional image_none = {0};

// optional
struct image_optional image_some(const struct image img) {
    return (struct image_optional) {.is_valid = true, .image = img };
}

struct image_optional image_create_empty(const uint64_t width, const uint64_t height){
    void* data = malloc(sizeof(struct pixel) * width * height);

    if (data == NULL) {
        return image_none;
    } else {
        return image_some((struct image) {.width = width, .height = height, .data = data});
    }
}

struct image_optional copy_image(const struct image source) {
    struct pixel *data = (struct pixel*) malloc(sizeof(struct pixel) * source.height * source.width);

    if (data == NULL) {
        return image_none;
    } else {
        for (uint64_t y = 0; y < source.height; ++y) {
            for (uint64_t x = 0; x < source.width; ++x) {
                data[source.width * y + x] = source.data[source.width * y + x];
            }
        }
        return image_some((struct image) {.width = source.width, .height = source.height, .data = data });
    }
}

void image_destroy(struct image *img){
    free(img->data);
}
