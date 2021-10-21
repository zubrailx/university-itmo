#include "file.h"

bool open_file(FILE **file, const char *name, const char *mode) {
    if (!file) return false;
    *file = fopen(name, mode);
    return *file == NULL;
}

bool close_file(FILE **file) {
    if (*file) { return false;}
    return fclose(*file);
}
