#ifndef IMAGE_C_SEPIA_H
#define IMAGE_C_SEPIA_H

#include "../image/image.h"

struct image_optional sepia_c(const struct image *img);

struct image_optional sepia_asm(const struct image *img);

#endif //IMAGE_C_SEPIA_H
