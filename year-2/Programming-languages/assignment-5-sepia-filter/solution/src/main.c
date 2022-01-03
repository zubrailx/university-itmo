#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "bmp/bmpio.h"
#include "file/file.h"
#include "tool/tool.h"
#include "tool/test.h"
#include "transform/rotate.h"
#include "transform/sepia.h"


static bool set_filenames(int argc, char **argv, char **in, char **out);
static int full_cycle(int argc, char **argv);
static int test();


int main(int argc, char **argv){
    return full_cycle(argc, argv);
//    return test();
}


static int test() {
    test_asm();
    test_c();

    return 0;
}

static int full_cycle(int argc, char **argv) {
//    test_program_arguments(argc, argv);

    // open files
    char *input_fn = "input.bmp";
    char *output_fn = "output.bmp";
//    set_filenames(argc, argv, &input_fn, &output_fn);

    FILE *input_file = NULL, *output_file = NULL;
    open_file(&input_file, input_fn, "rb");
    open_file(&output_file, output_fn, "wb");

    struct image image = {0};
    from_bmp(input_file, &image);

    // calculate time elapsed.
    struct rusage r;
    struct timeval start, end;
    getrusage(RUSAGE_SELF, &r );
    start = r.ru_utime;

//    struct image_optional image_sepia_1 = sepia_c(&image);
    struct image_optional image_sepia_2 = sepia_asm(&image);

    getrusage(RUSAGE_SELF, &r );
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    printf( "Time elapsed in microseconds: %ld\n", res );
    // end calculation

    to_bmp(output_file, &image_sepia_2.image);
//    to_bmp(output_file, &image_sepia_1.image);

    close_file(&input_file);
    close_file(&output_file);

//    image_destroy(&image_sepia_1.image);
    image_destroy(&image_sepia_2.image);
    image_destroy(&image);

    return 0;
}


bool set_filenames(int argc, char **argv, char **in, char **out) {
    if (argc < 3) {
        return 1;
    } else {
        *in = argv[1];
        *out = argv[2];
        return 0;
    }
}


