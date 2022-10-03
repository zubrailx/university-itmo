#pragma once

#include "sections/base.h"

typedef struct DatabaseHeader DatabaseHeader;
typedef struct DatabaseSection DatabaseSection;
typedef struct DatabaseTable DatabaseTable;
typedef struct DatabaseTableIndex DTIndex;
typedef struct DatabaseTableTyple DTTyple;
typedef struct DatabaseTableTypleInline DTTypleInline;
typedef struct DatabaseTableTypleColumnBase DTTColumnBase;
typedef struct DatabaseTableTypleColumnString DTTColumnString;
typedef struct DatabaseTableTypleColumnStringInline DTTColumnStringInline;

extern sectoff_t DATABASE_SECTION_SIZE;

// DatabaseSection
struct DatabaseHeader {
  BaseSection base_section;
  fileoff_t next;
  fileoff_t previous;
  /* Non-inclusive */
  sectoff_t off_index_last;
  /* From start to end */
  sectoff_t off_typle_start;
};

struct DatabaseSection {
  DatabaseHeader *header;
  void *body;
};

struct DatabaseTableIndex {
  sectoff_t off_start;
  sectoff_t off_end;
  bool is_cleared;
};

enum TableColumnTypes {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
};

union DTTName {
  char name_start[sizeof(SOPointer)];
  SOPointer pointer;
};

struct DatabaseTableTyple {
  fileoff_t fileoff;
  bool is_inline;
  union DTTName ptrname;
  // TableColumns
};

struct DatabaseTableTypleInline {
  fileoff_t fileoff;
  const char name[];
  // TableColumnsInline
};

struct DatabaseTableTypleColumnBase {
  /* TableColumnType */
  int8_t type;
  bool is_null;
  // special for data
};

struct DatabaseTableTypleColumnString {
  struct DatabaseTableTypleColumnBase base;
  bool is_inline; // if is_inline ? store string : store address (8 bytes)
  union DTTName ptrname;
};

struct DatabaseTableTypleColumnStringInline {
  struct DatabaseTableTypleColumnBase base;
  char *name;
};

void database_table_unload(DTTyple **dttyple);
DTTyple *database_table_load(Database *database, DatabaseSection *section,
                             sectoff_t offset);

DTTyple *database_table_create(Database *database, const char *name);
DTTyple *database_table_select(Database *database, const char *name);
DTTyple *database_table_delete(Database *database, const char *name);

sectoff_t database_section_get_body_offset(sectoff_t sectoff);
sectoff_t database_section_get_sectoff(sectoff_t bodyoff);

DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos);
void database_section_delete(Database *database, fileoff_t pos);

DatabaseSection *database_section_load_next(Database *database,
                                            const DatabaseSection *current);
DatabaseSection *database_section_load(Database *database,
                                       const fileoff_t offset);
void database_section_unload(DatabaseSection **dbs_ptr);
