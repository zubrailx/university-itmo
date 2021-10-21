#include "image.h"

#include <malloc.h>

bool image_malloc_data(struct image *img){
    uint64_t width = img->width;
    uint64_t height = img->height;
    img->data = malloc(sizeof(struct pixel) * width * height);
    return img->data == NULL;
}

void image_destroy(struct image *img){
    free(img->data);
}
