#pragma once

#include <stddef.h>

typedef struct dto_row_list {
  size_t length;
  struct dto_row_node *first, *last;
} dto_row_list;

struct dto_row_node *dto_row_node_next(struct dto_row_node *cur);
const void **dto_row_node_get(struct dto_row_node *cur);
