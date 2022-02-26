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
static void read_image_pixels_heap(FILE *in, struct image *img);
static enum write_status write_image_pixels(FILE* file, const struct image* img);
static enum write_status create_bmp_header(struct image const *img, struct bmp_header *header);
static size_t calculate_padding(size_t width);


enum read_status from_bmp(FILE *in, struct image *img) {
    if (!in) { return READ_STREAM_NULL; }
    if (!img) { return READ_TARGET_NULL; }

    struct bmp_header header = {0};
    enum read_status header_status = read_bmp_header(in, &header);
    if (header_status) { return header_status; }
    // create image (allocated)
    struct image_optional optional = image_create_empty(header.width, header.height);
    if (optional.is_valid) {
        *img = optional.image;
    } else {
        return READ_MALLOC_ERROR;
    }
    // set pointer to data and read
    fseek(in, header.data_offset, SEEK_SET);
    read_image_pixels_heap(in, img);

    return READ_OK;
}

enum write_status to_bmp(FILE *out, struct image *img) {
    if (!out) { return WRITE_STREAM_NULL; }
    if (!img) { return WRITE_SOURCE_NULL; }

    struct bmp_header header = {0};
    create_bmp_header(img, &header);
    // write header
    fwrite(&header, sizeof(struct bmp_header), 1, out);
    // set pointer to data and write
    fseek(out, header.data_offset, SEEK_SET);
    enum write_status write_status = write_image_pixels(out, img);
    if (write_status) { return write_status; }

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


static void read_image_pixels_heap(FILE *in, struct image *img) {
    int32_t padding = (int32_t) calculate_padding(img->width);
    for (size_t i = 0; i < img->height; ++i) {
        fread(img->data + i * (img->width), (size_t) (img->width) * sizeof(struct pixel), 1, in);
        fseek(in, padding, SEEK_CUR);
    }
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

static enum write_status write_image_pixels(FILE* file, const struct image* img) {
    size_t padding = calculate_padding(img->width);
    // malloc padding and set 0
    uint8_t *line_padding = malloc(padding);
    if (!line_padding) { return WRITE_ERROR; }
    for (size_t i = 0; i < padding; ++i){
        *(line_padding + i) = 0;
    }
    // write pixels
    if (img->data != NULL){
        for (size_t i = 0; i < img->height; ++i) {
            fwrite(img->data + i * img->width, img->width * sizeof(struct pixel), 1, file);
            fwrite(line_padding, padding, 1, file);
        }
    }
    free(line_padding);
    return WRITE_OK;
}

static size_t calculate_padding(size_t width) {
    return DOUBLE_WORD - width * (BIT_COUNT / 8) % DOUBLE_WORD;
}
