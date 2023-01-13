#include "dto_row.h"
#include <stdlib.h>
#include <dbms/util/define.h>
#include <malloc.h>

struct dto_row_node {
  const void **row;
  struct dto_row_node *next;
};

struct dto_row_node *dto_row_node_next(struct dto_row_node *cur) { return cur->next; }
const void **dto_row_node_get(struct dto_row_node *cur) { return cur->row; }

// LIST
struct dto_row_list *dto_row_list_construct() {
  struct dto_row_list *dto_row_list = my_malloc(struct dto_row_list);
  *dto_row_list = (struct dto_row_list){.length = 0, .first = NULL, .last = NULL};
  return dto_row_list;
}

void dto_row_list_destruct(struct dto_row_list **lst_ptr) {
  struct dto_row_node *cur, *next;
  struct dto_row_list *lst = *lst_ptr;
  cur = lst->first;

  while (lst->length--) {
    next = cur->next;
    free(cur);
    cur = next;
  }
  free(lst);
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
