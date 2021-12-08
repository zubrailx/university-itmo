#ifndef ASSIGNMENT_IMAGE_ROTATION_FILE_H
#define ASSIGNMENT_IMAGE_ROTATION_FILE_H

#include <stdbool.h>
#include <stdio.h>

bool open_file(FILE **file, const char *name, const char *mode);

bool close_file(FILE **file);

#endif //ASSIGNMENT_IMAGE_ROTATION_FILE_H
