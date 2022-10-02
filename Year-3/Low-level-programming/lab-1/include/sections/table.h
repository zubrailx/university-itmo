#pragma once

#include "base.h"
#include "sections/database.h"

extern uint32_t TABLE_SIZE;

typedef struct TableHeader {
  BaseSection base_section;
  uint32_t page_count;
  fileoff_t page_last_filled;
  fileoff_t page_first;
  /* Offset in file to the hash */
  fileoff_t hash;
} TableHeader;

typedef struct TableSection {
  TableHeader header;
  void *body;
} TableSection;

enum TableColumnTypes {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
};

TableSection *database_table_create(Database *database, const char *name);
TableSection *database_table_select(Database *database, const char *name);
void database_table_delete(Database *database, const char *name);

void table_add_column(Database *database, const char *table, const char *column,
                      enum TableColumnTypes type);
void table_delete_column(Database *database, const char *table,
                         const char *column);
