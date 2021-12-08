#ifndef LAB3_BMPIO_H
#define LAB3_BMPIO_H

#include <stdint.h>
#include <stdio.h>

#include "../image/image.h"

#pragma pack(push, 1)
struct bmp_header {
    uint16_t signature;
    uint32_t filesize;
    uint32_t reserved;
    uint32_t data_offset;
    uint32_t size;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bit_count;
    uint32_t compression;
    uint32_t image_size;
    uint32_t x_pixels_per_m;
    uint32_t y_pixels_per_m;
    uint32_t colors_used;
    uint32_t colors_important;
};
#pragma pack(pop)

enum read_status {
    READ_OK = 0,
    READ_INVALID_HEADER,
    READ_STREAM_NULL,
    READ_TARGET_NULL,
    READ_ERROR
};

enum write_status {
    WRITE_OK = 0,
    WRITE_STREAM_NULL,
    WRITE_SOURCE_NULL,
    WRITE_ERROR
};


enum write_status to_bmp(FILE *out, struct image *img);

enum read_status from_bmp(FILE *in, struct image *img);

#endif //LAB3_BMPIO_H
