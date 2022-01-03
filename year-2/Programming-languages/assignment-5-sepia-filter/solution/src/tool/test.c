#include "test.h"

#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "../bmp/bmpio.h"
#include "../file/file.h"
#include "../transform/sepia.h"


static struct timeval check_timer(struct rusage *r);
static void print_timer(struct timeval start, struct timeval end, const char *where);

int test_c() {
    struct rusage r;
    struct timeval start, end;

    struct image_optional img_opt = image_create_empty(16384, 16384);
    if (!img_opt.is_valid) { return -1; }

    start = check_timer(&r);

    img_opt = sepia_c(&img_opt.image);
    if (!img_opt.is_valid) { return -1; }

    end = check_timer(&r);
    print_timer(start, end, "c_func");

    image_destroy(&img_opt.image);
    return 0;
}

int test_asm() {
    struct rusage r;
    struct timeval start, end;

    struct image_optional img_opt = image_create_empty(16384, 16384);
    if (!img_opt.is_valid) { return -1; }

    start = check_timer(&r);

    img_opt = sepia_asm(&img_opt.image);
    if (!img_opt.is_valid) { return -1; }

    end = check_timer(&r);
    print_timer(start, end, "asm_func");

    image_destroy(&img_opt.image);
    return 0;
}


static struct timeval check_timer(struct rusage *r) {
    getrusage(RUSAGE_SELF, r);
    return r->ru_utime;
}

static void print_timer(struct timeval start, struct timeval end, const char *const where) {
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) + end.tv_usec - start.tv_usec;
    printf("Time elapsed in %s in microseconds: %ld\n", where, res);
}
