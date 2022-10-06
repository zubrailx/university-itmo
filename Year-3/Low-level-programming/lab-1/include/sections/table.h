#pragma once

// #include "sections/database.h"

// typedef struct TableHeader TableHeader;
// typedef struct TableSection TableSection;

// extern sectoff_t TABLE_SECTION_SIZE;

// struct TableHeader {
//   BaseSection base_section;
//   uint32_t page_count;
//   fileoff_t page_last_filled;
//   fileoff_t page_first;
//   /* Offset in file to the hash */
//   fileoff_t hash;
// };

// struct TableSection {
//   TableHeader *header;
//   void *body;
// };

// TableSection *table_section_load(Database *database, const fileoff_t offset);
// void table_section_unload(TableSection **ts_ptr);

// TableSection *table_section_create(Database *database, DTTyple *dttyple);
// TableSection *table_section_select(Database *database, DTTyple *dttyple);
// TableSection *table_section_delete(Database *database, DTTyple *dttyple);

// TableSection *table_create(Database *database, const char *name);
// TableSection *table_select(Database *database, const char *name);
// void *table_delete(Database *database, const char *name);

// void table_add_column(Database *database, const char *table, const char
// *column,
//                       enum TableColumnTypes type);
// void table_delete_column(Database *database, const char *table,
//                          const char *column);
