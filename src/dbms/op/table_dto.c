#include "table_dto.h"

#include <malloc.h>
#include <string.h>

static dto_table_column *dto_table_column_construct(char *name,
                                                    enum table_column_types *type,
                                                    struct dpt_col_limits *lims) {
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

struct dto_table *dto_table_construct(char *name) {
  struct dto_table *table = my_malloc(dto_table);
  table->header.name = strdup(name);
  table->first = NULL;
  table->last = NULL;
  return table;
}

void dto_table_destruct(struct dto_table **typle_ptr) {
  free((*typle_ptr)->header.name);
  dto_table_column *cur = (*typle_ptr)->first;
  while (cur != NULL) {
  dto_table_column *next = cur->next;
    dto_table_column_destruct(&cur);
    cur = next;
  }
  free(*typle_ptr);
  *typle_ptr = NULL;
}

void dto_table_add_column(struct dto_table *typle, char *name,
                          enum table_column_types type, struct dpt_col_limits lims) {
  dto_table_column *col = dto_table_column_construct(name, &type, &lims);
  if (typle->first == NULL) {
    typle->first = col;
    typle->last = col;
  } else {
    typle->last->next = col;
    typle->last = col;
  }
}
