#include "util.h"

#include <stdio.h>
#include <stdlib.h>

struct list *list_ctr(void) {
  struct list *list = (struct list *)calloc(sizeof(struct list), 1);
  list->end = (struct list_entry *)calloc(sizeof(struct list_entry), 1);
  list->begin = list->end;
  return list;
}

void list_entry_dtr(struct list_entry **entry_ptr) {
  struct list_entry *entry = *entry_ptr;
  free(entry);
  *entry_ptr = NULL;
}

void list_dtr(struct list **list_ptr) {
  struct list *list = *list_ptr;
  struct list_entry *cur, *next;

  for (cur = list->begin; cur != list->end;) {
    next = cur->next;
    list_entry_dtr(&cur);
    cur = next;
  }
  list_entry_dtr(&list->end);

  free(list);
  *list_ptr = NULL;
}

struct list_entry *list_insert(struct list *list, size_t pos, void *data) {
  struct list_entry *ins_before = list->begin;

  for (size_t i = 0; i < pos; ++i) {
    ins_before = ins_before->next;
  }

  struct list_entry *elem =
      (struct list_entry *)calloc(sizeof(struct list_entry), 1);
  elem->data = data;
  elem->next = ins_before;

  if (pos == 0) {
    list->begin = elem;
  } else {
    struct list_entry *ins_after = list->begin;
    for (size_t i = 1; i < pos; ++i) {
      ins_after = ins_after->next;
    }
    ins_after->next = elem;
  }
  return elem;
}

struct list_entry *list_erase(struct list *list, struct list_entry *elem) {
  struct list_entry *cur = list->begin;
  struct list_entry *rem = NULL;

  if (elem == list->begin) {
    list->begin = cur->next;
    rem = cur;
  } else {
    struct list_entry *prev = cur;
    for (cur = cur->next; cur != list->end; prev = cur, cur = cur->next) {
      if (cur == elem) {
        prev->next = cur->next;
        rem = cur;
        break;
      }
    }
  }

  if (rem != NULL) {
    struct list_entry *next = rem->next;
    list_entry_dtr(&rem);
    return next;
  }

  return list->end;
}

struct list_entry *list_find(struct list *list, pred_func pred, void *lhs) {
  struct list_entry *cur;
  for (cur = list->begin; cur != list->end; cur = cur->next) {
    if (pred(lhs, cur->data)) {
      return cur;
    }
  }
  return list->end;
}

size_t list_size(struct list *list) {
  struct list_entry *cur;
  size_t size = 0;
  for (cur = list->begin; cur != list->end; cur = cur->next, ++size)
    ;
  return size;
}

int list_empty(struct list *list) { return list->begin == list->end; }

void print_list(struct list *list) {
  for (struct list_entry *it = list->begin; it != list->end; it = it->next) {
    printf("{%p}", it->data);
  }
  printf("\n");
}
