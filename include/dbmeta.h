#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "dbtypes.h"
#include "util.h"
#include "sections/meta.h"

/* Stored in RAM */
my_defstruct(Database);
struct Database {
  FILE *file;
  char *name;
  bool is_opened;
  struct DatabaseMeta dst;
};

// Function declarations
Database database_create(const char *filename);
void database_flush(const Database *database);
void database_alter(const Database *database, const char *meta);
void database_drop(Database *database);

Database database_open(const char *filename);
void database_close(Database *database);
void database_remove(Database *database);
