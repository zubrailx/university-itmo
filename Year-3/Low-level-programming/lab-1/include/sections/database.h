#pragma once

#include "base.h"

#include <stdbool.h>
#include <stdio.h>

// #define DATABASE_SIZE_VAL 4096;
extern sectoff_t DATABASE_SIZE;

/* Stored in RAM */
typedef struct Database {
  FILE *file;
  char *name;
  bool is_opened;
  bool is_corrupted;
  /* Offset to first and last DatabaseSection */
  fileoff_t ds_first;
  fileoff_t ds_last;
  fileoff_t pos_empty;
} Database;
/* Stored in file */
typedef struct DatabaseStored {
  bool is_corrupted;
  fileoff_t ds_first;
  fileoff_t ds_last;
  fileoff_t pos_empty;
} DatabaseStored;

// DatabaseSection
typedef struct DatabaseHeader {
  BaseSection base_section;
  fileoff_t next;
  fileoff_t previous;
  /* Non-inclusive */
  sectoff_t off_table_last;
} DatabaseHeader;

typedef struct DatabaseTable {
  // uint8_t name_size;
  fileoff_t table_section;
} DatabaseTable;

typedef struct DatabaseSection {
  DatabaseHeader header;
  void *body;
} DatabaseSection;

DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos);
void database_section_delete(Database *database, fileoff_t pos);

DatabaseSection *database_section_load_next(Database *database,
                                           const DatabaseSection *current);

DatabaseSection *database_section_load(Database *database,
                                       const fileoff_t offset);
void database_section_unload(DatabaseSection **dbs_ptr);

Database database_create(const char *filename);
Database database_open(const char *filename);

void database_close(Database *database);
