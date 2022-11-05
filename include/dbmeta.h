#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include "util.h"

typedef uint64_t fileoff_t;
typedef uint32_t sectoff_t;
typedef uint32_t bodyoff_t;

#define SECTION_OFFSET_NULL 0
// Because 0 points to current page
#define SECTION_START_INDEX 1
#define SECTION_CURRENT_PTR 0

/* Stored in file */
my_defstruct(DatabaseMeta);
struct DatabaseMeta {
  bool is_corrupted;
  fileoff_t ds_first; // first DatabaseSection
  fileoff_t ds_last;
  fileoff_t da_last; // first DataSection
  fileoff_t da_first;
  fileoff_t pos_empty;
};

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
