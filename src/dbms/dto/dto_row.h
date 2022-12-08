#pragma once

#include <stddef.h>

struct dto_row_node {
  const void **row;
  struct dto_row_node *next;
};

typedef struct dto_row_list {
  size_t length;
  struct dto_row_node *first, *last;
} dto_row_list;

struct dto_row_list dto_row_list_construct();
void dto_row_list_destruct(struct dto_row_list *lst);
void dto_row_list_append(dto_row_list *lst, const void *row[]);
