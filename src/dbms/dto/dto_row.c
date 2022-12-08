#include "dto_row.h"
#include <stdlib.h>

struct dto_row_list dto_row_list_construct() {
  return (dto_row_list){.length = 0, .first = NULL, .last = NULL};
}

void dto_row_list_destruct(struct dto_row_list *lst) {
  struct dto_row_node *cur, *next;
  cur = lst->first;

  while (lst->length--) {
    next = cur->next;
    free(cur);
    cur = next;
  }
}

void dto_row_list_append(dto_row_list *lst, const void *row[]) {
  struct dto_row_node *node = malloc(sizeof(struct dto_row_node));
  *node = (struct dto_row_node){.next = NULL, .row = row};
  if (lst->length == 0) {
    lst->first = node;
    lst->last = node;
  } else {
    lst->last->next = node;
    lst->last = node;
  }
  ++lst->length;
}
