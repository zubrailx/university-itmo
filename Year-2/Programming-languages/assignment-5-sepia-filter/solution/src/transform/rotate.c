#include "rotate.h"


struct image_optional rotate(const struct image* const source){
    uint64_t width = source->width;
    uint64_t height = source->height;
    struct pixel *data = source->data;
    // create new image
    struct image_optional optional = image_create_empty(width, height);
    if (!optional.is_valid) {
        return image_none;
    }

    struct pixel *copy_data = optional.image.data;

    for (uint64_t y = 0; y < height; ++y)
        for (uint64_t x = 0; x < width; ++x)
            *(copy_data + x * height + (height - 1 - y)) = *(data + y * width + x);

    return image_some((struct image) {
        .width = optional.image.height,
        .height = optional.image.width,
        .data = copy_data
    });
}
