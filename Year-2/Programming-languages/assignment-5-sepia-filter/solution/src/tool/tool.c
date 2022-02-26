#include "tool.h"


void test_program_arguments(int argc, char **argv) {
    for (size_t i = 0; i < argc; ++i) {
        puts(argv[i]);
    }
}

void print_bmp_header(struct bmp_header *header) {
    printf("signature\t\t: %u\n", header->signature);
    printf("filesize\t\t: %u\n", header->filesize);
    printf("reserved\t\t: %u\n", header->reserved);
    printf("data_offset\t\t: %u\n", header->data_offset);
    printf("size\t\t\t: %u\n", header->size);
    printf("width\t\t\t: %u\n", header->width);
    printf("height\t\t\t: %u\n", header->height);
    printf("planes\t\t\t: %hu\n", header->planes);
    printf("bit_count\t\t: %hu\n", header->bit_count);
    printf("compression\t\t: %u\n", header->compression);
    printf("image_size\t\t: %u\n", header->image_size);
    printf("x_pixels_per_m\t\t: %u\n", header->x_pixels_per_m);
    printf("y_pixels_per_m\t\t: %u\n", header->y_pixels_per_m);
    printf("colors_used\t\t: %u\n", header->colors_used);
    printf("colors_important\t: %u\n", header->colors_important);
}
