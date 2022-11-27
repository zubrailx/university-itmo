#include "table.h"

#include <malloc.h>
#include <string.h>

#include <util/define.h>

static dto_table_column *
dto_table_column_construct(const char *name, const enum dto_table_column_type *type,
                           const struct dto_table_column_limits *lims) {
  dto_table_column *col = my_malloc(dto_table_column);
  col->name = strdup(name);
  col->lims = *lims;
  col->type = *type;
  col->next = NULL;
  return col;
}

static void dto_table_column_destruct(struct dto_table_column **col) {
  free((*col)->name);
  free(*col);
  *col = NULL;
}

struct dto_table *dto_table_construct(const char *name) {
  struct dto_table *table = my_malloc(dto_table);
  table->name = strdup(name);
  table->first = NULL;
  table->last = NULL;
  return table;
}

void dto_table_destruct(struct dto_table **typle_ptr) {
  if (*typle_ptr == NULL) return;
  free((*typle_ptr)->name);
  dto_table_column *cur = (*typle_ptr)->first;
  while (cur != NULL) {
    dto_table_column *next = cur->next;
    dto_table_column_destruct(&cur);
    cur = next;
  }
  free(*typle_ptr);
  *typle_ptr = NULL;
}

void dto_table_add_column(struct dto_table *table, const char *name,
                          const enum dto_table_column_type type,
                          const struct dto_table_column_limits lims) {
  dto_table_column *col = dto_table_column_construct(name, &type, &lims);
  if (table->first == NULL) {
    table->first = col;
    table->last = col;
  } else {
    table->last->next = col;
    table->last = col;
  }
}

int dto_table_columns(const dto_table *table) {
  int cnt = 0;
  dto_table_column *cur = table->first;
  while (cur != NULL) {
    cur = cur->next;
    ++cnt;
  }
  return cnt;
}
