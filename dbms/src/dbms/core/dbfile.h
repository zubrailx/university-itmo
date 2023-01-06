#pragma once

#include <stdbool.h>
#include <stdio.h>

/* Stored in RAM */
typedef struct dbfile {
  FILE *file;
  char *fname;
} dbfile;

FILE *dbfile_file(dbfile *dbfile);
const char *dbfile_name(dbfile *dbfile);

dbfile *dbfile_construct(const char *fname, bool do_trunc);
int dbfile_destruct(dbfile **dbfile);

int dbfile_flush(dbfile *dbfile);
int dbfile_close(dbfile *dbfile);
int dbfile_remove(dbfile *dbfile);
