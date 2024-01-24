#ifndef __LAB4_UTIL__H
#define __LAB4_UTIL__H

#include <stddef.h>

// To not use pointers - rewrite in GENERIC
struct list_entry {
  void *data;
  struct list_entry *next;
};

struct list {
  struct list_entry *begin;
  struct list_entry *end;
};

typedef int (*pred_func)(void *lhs, void *rhs);

struct list *list_ctr(void);
void list_dtr(struct list **list_ptr);

struct list_entry *list_insert(struct list *list, size_t pos, void *data);
struct list_entry *list_erase(struct list *list, struct list_entry *elem);
struct list_entry *list_find(struct list *list, pred_func pred, void *lhs);
size_t list_size(struct list *list);
int list_empty(struct list *list);

void print_list(struct list *list);
#endif
