#include "sections/table.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

uint32_t TABLE_SIZE = 4096;

static TableSection *read_malloc_table(Database *database, fileoff_t offset) {
  TableSection *table = my_malloc(TableSection);
  FILE *file = database->file;
  // Read table header
  fseek(file, offset, SEEK_SET);
  assert(fread(&table->header, sizeof(TableHeader), 1, file));
  // Read table body
  sectoff_t body_size = table->header.base_section.size - sizeof(TableHeader);
  table->body = malloc(body_size);

  return table;
}

TableSection *database_table_create(Database *database, const char *name) {
  assert(database->is_opened);
  if (database_table_select(database, name) == NULL) {
    return NULL;
  } 
  
}

TableSection *database_table_select(Database *database, const char *name) {
  assert(database->is_opened);
  DatabaseSection *ds = database_section_load(database, database->ds_first);
  while (ds != NULL) {
    sectoff_t off_start = sizeof(DatabaseHeader);
    sectoff_t off_last = ds->header.off_table_last;

    while (off_start < off_last) {
      DatabaseTable *dt = (void *)ds + off_start;
      const char *table_name = (void *)dt + sizeof(DatabaseTable);
      if (strcmp(name, table_name) == 0) {
        return read_malloc_table(database, dt->table_section);
      }
      size_t entry_size = sizeof(DatabaseTable) + strlen(table_name) + 1;
      off_start += entry_size;
    }
    ds = database_section_load_next(database, ds);
  }
  return NULL;
}

void database_table_delete(Database *database, const char *name) {

}

