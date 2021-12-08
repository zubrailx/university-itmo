#include <stdbool.h>
#include <stdio.h>

#include "bmp/bmpio.h"
#include "file/file.h"
#include "tool/tool.h"
#include "transform/rotate.h"

static inline bool set_filenames(int argc, char **argv, char **in, char **out);


int main(int argc, char **argv) {
    test_program_arguments(argc, argv);

    char *input_fn = NULL, *output_fn = NULL;
    if (set_filenames(argc, argv, &input_fn, &output_fn)) {
        fprintf(stderr, "Invalid program arguments\n");
        return 1;
    }

    FILE *input_file = NULL, *output_file = NULL;
    bool in_bool, out_bool;

    in_bool = open_file(&input_file, input_fn, "rb");
    out_bool = open_file(&output_file, output_fn, "wb");
    if (in_bool|| out_bool) {
        fprintf(stderr, "Cannot open files with given arguments\n");
        return 1;
    }

    struct image image = {0};
    if (from_bmp(input_file, &image)){
        fprintf(stderr, "Cannot convert bmp file");
        return 1;
    }
    struct image new_image = rotate(image);

    if (to_bmp(output_file, &new_image)){
        fprintf(stderr, "Cannot write image to file");
        return 1;
    }

    in_bool = close_file(&input_file);
    out_bool = close_file(&output_file);
    if (in_bool || out_bool){
        fprintf(stderr, "Cannot close files\n");
        return 1;
    }

    image_destroy(&new_image);
    image_destroy(&image);
    return 0;
}


static inline bool set_filenames(const int argc, char **argv, char **in, char **out) {
    if (argc < 3) {
        return 1;
    } else {
        *in = argv[1];
        *out = argv[2];
        return 0;
    }
}
