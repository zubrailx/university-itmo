#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "../util/define.h"

/* Stored in RAM */
my_defstruct(dbfile);
struct dbfile {
  FILE *file;
  char *fname;
  bool is_opened;
  bool is_writable;
};

FILE *dbfile_file(dbfile *dbfile);
const char *dbfile_name(dbfile *dbfile);
bool dbfile_is_opened(dbfile *dbfile);
bool dbfile_is_writable(dbfile *dbfile);

dbfile *dbfile_construct(const char *fname, bool do_trunc);
void dbfile_destruct(dbfile **dbfile);

bool dbfile_flush(dbfile *dbfile);
bool dbfile_close(dbfile *dbfile);
void dbfile_remove(dbfile *dbfile);

