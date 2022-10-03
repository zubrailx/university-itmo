#pragma once

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

typedef uint64_t fileoff_t;
typedef uint32_t sectoff_t;

typedef struct Database Database;
typedef struct DatabaseStored DatabaseStored;

#define SECTION_OFFSET_NULL 0
// Because 0 points to current page
#define SECTION_START_INDEX 1
#define CURRENT_SECTION_PTR 0

/* Stored in file */
struct DatabaseStored {
  bool is_corrupted;
  fileoff_t ds_first;
  fileoff_t ds_last;
  fileoff_t pos_empty;
};
/* Stored in RAM */
struct Database {
  FILE *file;
  char *name;
  bool is_opened;
  DatabaseStored ds;
};

Database database_create(const char *filename);
Database database_open(const char *filename);
void database_close(Database *database);
