#pragma once

#include "sections/base.h"

extern sectoff_t DATABASE_SIZE;

typedef struct DatabaseHeader DatabaseHeader;
typedef struct DatabaseSection DatabaseSection;
typedef struct DatabaseTable DatabaseTable;
typedef struct DatabaseTableIndex DTIndex;
typedef struct DatabaseTableTyple DTTyple;
typedef struct DatabaseTableTypleColumnBase DTTColumnBase;
typedef struct DatabaseTableTypleColumnString DTTColumnString;

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

union DTTCName {
  // min 8 bytes of name
  char name_start[8];
  fileoff_t fileoff;
};

struct DatabaseTableTyple {
  fileoff_t fileoff;
  bool is_inline;
  union DTTCName table_name;
  // TableColumns
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
  union DTTCName u_name;
};

DatabaseSection *database_section_create(Database *database,
                                         DatabaseSection *previous,
                                         fileoff_t previous_pos);
void database_section_delete(Database *database, fileoff_t pos);

DatabaseSection *database_section_load_next(Database *database,
                                            const DatabaseSection *current);

DatabaseSection *database_section_load(Database *database,
                                       const fileoff_t offset);
void database_section_unload(DatabaseSection **dbs_ptr);
