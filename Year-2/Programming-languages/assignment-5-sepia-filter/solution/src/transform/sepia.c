#include "sepia.h"

#include <stdio.h>


extern void sepia_pixels_asm(struct pixel *input, struct pixel *output, uint64_t count);


static void sepia_one(struct pixel *input, struct pixel *output);
static unsigned char sat(uint64_t x);

// create  static pixel array
static float pixel_array_float[256];


struct image_optional sepia_c(const struct image *const img) {
    // init static pixel array
    for (size_t i = 0; i < 256; ++i) {
        pixel_array_float[i] = (float) i;
    }

    struct image_optional optional = copy_image(img);
    if (!optional.is_valid) return image_none;

//    for (uint32_t i = 0; i < 100; ++i){
        for (uint32_t y = 0; y < img->height; ++y)
            for (uint32_t x = 0; x < img->width; ++x){
//                struct pixel input = img->data[img->width * y + x];
//                printf("%d(%d; %d) - r:%d, g:%d, b:%d -> ", (uint32_t)(img->width * y + x), x, y, input.r, input.g, input.b);

                sepia_one(&img->data[img->width * y + x], &optional.image.data[img->width * y + x]);
//                struct pixel output = optional.image.data[img->width * y + x];
//                printf("r:%d, g:%d, b:%d\n", output.r, output.g, output.b);
            }
//    }

    return optional;
}

struct image_optional sepia_asm(const struct image *const img) {
    struct image_optional optional = image_create_empty(img->width, img->height);
    if (!optional.is_valid) return image_none;

//    for (uint32_t i = 0; i < 100; ++i) {
        sepia_pixels_asm(img->data, optional.image.data, img->width * img->height);
//    }

    return optional;
}

static unsigned char sat(uint64_t x) {
    if (x < 256) return x;
    return 255;
}

static void sepia_one(struct pixel *const input, struct pixel *output) {
    static const float c[3][3] = {
            {.393f, .769f, .189f},
            {.349f, .686f, .168f},
            {.272f, .543f, .131f}};


    output->r = sat((uint64_t)(
            pixel_array_float[input->r] * c[0][0] +
            pixel_array_float[input->g] * c[0][1] +
            pixel_array_float[input->b] * c[0][2]));
    output->g = sat((uint64_t)(
            pixel_array_float[input->r] * c[1][0] +
            pixel_array_float[input->g] * c[1][1] +
            pixel_array_float[input->b] * c[1][2]));
    output->b = sat((uint64_t)(
            pixel_array_float[input->r] * c[2][0] +
            pixel_array_float[input->g] * c[2][1] +
            pixel_array_float[input->b] * c[2][2]));
}

