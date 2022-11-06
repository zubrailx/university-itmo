#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "dbtypes.h"
#include "page/meta.h"
#include "util.h"

/* Stored in RAM */
my_defstruct(database);
struct database {
  FILE *file;
  char *name;
  bool is_opened;
  struct database_meta dst;
};

void database_alter(const database *database, const char *meta);
void database_drop(database **database);
