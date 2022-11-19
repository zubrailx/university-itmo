#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "dbtypes.h"
#include "meta/meta.h"
#include "util.h"

/* Stored in RAM */
my_defstruct(database);
struct database {
  FILE *file;
  char *fname;
  bool is_opened;
  bool is_writable;
};

void database_alter(const database *database);
void database_drop(database **database);

FILE *database_get_file(const database *);
