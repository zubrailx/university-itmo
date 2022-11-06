#pragma once

#include "../sso.h"
#include "base.h"

extern pageoff_t DATABASE_PAGE_SIZE;

// DatabaseSection
my_defstruct(database_header);
struct database_header {
  struct base_page base;
  fileoff_t next;
  fileoff_t previous;
  /* Non-inclusive body offset */
  bodyoff_t index_last;
  /* From start to end */
  bodyoff_t typle_start;
};

my_defstruct(database_page);
struct database_page {
  struct database_header header;
  char body[];
};

my_defstruct(database_index);
struct database_index {
  bodyoff_t start;
  bodyoff_t end;
  bool is_cleared;
};

enum table_column_types {
  COLUMN_TYPE_INT = 0,
  COLUMN_TYPE_FLOAT = 1,
  COLUMN_TYPE_STRING = 2,
  COLUMN_TYPE_BOOL = 3,
};

// database_page_typle
my_defstruct(dpt_header);
struct dpt_header {
  fileoff_t fileoff;
  size_t cols;
  struct page_sso sso;
};

my_defstruct(dpt_column_limits);
struct dpt_column_limits {
  bool is_null;
  bool is_unique;
};

my_defstruct(dpt_column);
struct dpt_column {
  /* TableColumnType */
  int8_t type;
  struct dpt_column_limits limits;
  struct page_sso sso;
};

my_defstruct(dp_typle);
struct dp_typle {
  struct dpt_header header;
  struct dpt_column columns[];
};

// dpTyple in RAM
my_defstruct(dpt_header_ram);
struct dpt_header_ram {
  fileoff_t fileoff;// offset to table
  size_t cols;

  bool changed;// does name in file is pointed

  bool is_inline;
  struct po_ptr so;
  char ssize[SSO_SSIZE_SIZE];
  char *name;
};

// linked list
my_defstruct(dpt_column_ram);
struct dpt_column_ram {
  struct dpt_column_ram *next;

  int8_t type; /* TableColumnType */
  struct dpt_column_limits limits;

  bool changed;// does name in file is pointed

  bool is_inline;
  struct po_ptr so;
  char ssize[SSO_SSIZE_SIZE];
  char *name;
};

my_defstruct(dp_typle_ram);
struct dp_typle_ram {
  struct dpt_header_ram header;
  struct dpt_column_ram *columns;
};

// HELPER
my_defstruct(database_page_wr);
struct database_page_wr {// wrapper
  fileoff_t fileoff;
  struct database_page *dp;// linked list
};

my_defstruct(dp_typle_ram_wr);
struct dp_typle_ram_wr {
  fileoff_t ifileoff;// section where index is located
  bodyoff_t ibodyoff;// offset to index
  struct dp_typle_ram *typle;
};

// Function declarations
size_t dp_get_space_left(const database_page *dbs);
size_t dp_get_body_size(const database_page *dbs);
bodyoff_t dp_get_bodyoff(pageoff_t sectoff);
pageoff_t dp_get_pageoff(bodyoff_t bodyoff);

// DatabaseSection specific
database_page_wr dp_create(database *db, database_page *prev, fileoff_t prev_pos);
void dp_alter(database *database, const fileoff_t fileoff, const database_page *dp);
void dp_alter_bodyoff(database *database, const void *data, fileoff_t fileoff,
                      bodyoff_t offset, size_t size);
void dp_alter_bodyoff_sync(database *database, database_page *dp, const void *data,
                           fileoff_t fileoff, bodyoff_t offset, size_t size);
void dp_drop(database *database, fileoff_t pos);

database_page_wr dp_load_next(database *database, const database_page *current);
database_page *dp_load(database *database, fileoff_t fileoff);
void dp_unload(database_page **dp);

// database_page Typle and Index specific
dp_typle_ram *dp_typle_ram_load(database *db, database_page *section,
                                const database_index *index);
void dp_typle_ram_unload(dp_typle_ram **const ram_ptr);

dp_typle_ram dp_typle_ram_create(char *name, size_t nsize, dpt_column_ram *column,
                                 size_t cols);
dpt_column_ram dp_typle_column_ram_create(int8_t type, dpt_column_limits, char *name,
                                          size_t size);

dp_typle *dp_typle_ram_to_typle(const dp_typle_ram *typle);

dp_typle_ram_wr dp_table_select(database *database, const char *name);
dp_typle_ram_wr dp_table_create(database *database, dp_typle_ram *ram);
dp_typle_ram_wr dp_table_drop(database *database, const char *name);
