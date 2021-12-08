#include "bmpio.h"

#include <malloc.h>

static uint32_t SIGNATURE = 19778;
static uint32_t RESERVED = 0;
static uint32_t HEADER_SIZE = 40;
static uint16_t PLANES = 1;
static uint32_t COMPRESSION = 0;
static uint32_t PIXEL_PER_M = 2834;
static uint32_t COLORS_USED = 0;
static uint32_t COLORS_IMPORTANT = 0;
static uint32_t DOUBLE_WORD = 4;
static size_t BIT_COUNT = 24;


static enum read_status read_bmp_header(FILE *in, struct bmp_header *header_ptr);

static enum read_status read_image_pixels_heap(FILE *in, struct image *img);

static enum write_status create_bmp_header(struct image const *img, struct bmp_header *header);

static size_t calculate_padding(size_t width);


enum read_status from_bmp(FILE *in, struct image *img) {
    if (!in) { return READ_STREAM_NULL; }
    if (!img) { return READ_TARGET_NULL; }

    struct bmp_header header = {0};
    enum read_status header_status = read_bmp_header(in, &header);
    if (header_status) { return header_status; }

    img->height = header.height;
    img->width = header.width;
    fseek(in, header.data_offset, SEEK_SET);
    enum read_status pixel_status = read_image_pixels_heap(in, img);
    if (pixel_status) { return pixel_status; }

    return READ_OK;
}

enum write_status to_bmp(FILE *out, struct image *img) {
    if (!out) { return WRITE_STREAM_NULL; }
    if (!img) { return WRITE_SOURCE_NULL; }

    struct bmp_header header = {0};
    create_bmp_header(img, &header);
//    if (header_status) { return header_status; }

    fwrite(&header, sizeof(struct bmp_header), 1, out);
    fseek(out, header.data_offset, SEEK_SET);

    size_t padding = calculate_padding(img->width);
    uint8_t *line_padding = malloc(padding);
    if (!line_padding) { return WRITE_ERROR; }
    for (size_t i = 0; i < padding; ++i){
        *(line_padding + i) = 0;
    }
    if (img->data != NULL){
        for (size_t i = 0; i < img->height; ++i) {
            fwrite(img->data + i * img->width, img->width * sizeof(struct pixel), 1, out);
            fwrite(line_padding, padding, 1, out);
        }
    }
    free(line_padding);
    return WRITE_OK;
}


static enum read_status read_bmp_header(FILE *in, struct bmp_header *header_ptr) {
    fseek(in, 0, SEEK_END);
    size_t f_size = ftell(in);
    if (f_size < sizeof(struct bmp_header)) { return READ_INVALID_HEADER; }

    rewind(in);
    fread(header_ptr, sizeof(struct bmp_header), 1, in);
    return READ_OK;
}

static enum read_status read_image_pixels_heap(FILE *in, struct image *img) {

    if (image_malloc_data(img)) { return READ_ERROR; }

    int32_t padding = (int32_t) calculate_padding(img->width);
    for (size_t i = 0; i < img->height; ++i) {
        fread(img->data + i * (img->width), (size_t) (img->width) * sizeof(struct pixel), 1, in);
        fseek(in, padding, SEEK_CUR);
    }
    return READ_OK;
}

static enum write_status create_bmp_header(struct image const *img, struct bmp_header *header) {
    size_t padding = calculate_padding(img->width);
    header->signature = SIGNATURE;
    header->image_size = (img->width * sizeof(struct pixel) + padding) * img->height;
    header->filesize = header->image_size + sizeof(struct bmp_header);
    header->reserved = RESERVED;
    header->data_offset = sizeof(struct bmp_header);
    header->size = HEADER_SIZE;
    header->width = img->width;
    header->height = img->height;
    header->planes = PLANES;
    header->bit_count = BIT_COUNT;
    header->compression = COMPRESSION;
    header->x_pixels_per_m = PIXEL_PER_M;
    header->y_pixels_per_m = PIXEL_PER_M;
    header->colors_used = COLORS_USED;
    header->colors_important = COLORS_IMPORTANT;
    return WRITE_OK;
}

static inline size_t calculate_padding(size_t width) {
    return DOUBLE_WORD - width * (BIT_COUNT / 8) % DOUBLE_WORD;
}
