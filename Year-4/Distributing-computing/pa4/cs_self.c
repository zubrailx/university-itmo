#include "cs_self.h"
#include <stdlib.h>

static int local_id_pred(void *lhs, void *rhs) {
  local_id *lhs_id = lhs;
  local_id *rhs_id = rhs;
  return *lhs_id == *rhs_id;
}

// -1 - less, 0 - eq, 1 - greater
int cs_entry_compare(cs_entry *lhs, cs_entry *rhs) {
  if (lhs->time > rhs->time)
    return 1;
  else if (lhs->time < rhs->time)
    return -1;
  else if (lhs->id > rhs->id)
    return 1;
  else if (lhs->id < rhs->id)
    return -1;
  else
    return 0;
}

cs_self *cs_self_ctr(ipc_self *ipc) {
  cs_self *cs = malloc(sizeof(cs_self));
  cs->ipc = ipc;
  cs->cs_queue = list_ctr();
  cs->act_prs = list_ctr();
  return cs;
}

void cs_self_dtr(cs_self **cs_ptr) {
  cs_self *cs = *cs_ptr;

  for (struct list_entry *it = cs->act_prs->begin; it != cs->act_prs->end;
       it = it->next) {
    free(it->data);
  }
  list_dtr(&cs->act_prs);

  for (struct list_entry *it = cs->cs_queue->begin; it != cs->cs_queue->end;
       it = it->next) {
    free(it->data);
  }
  list_dtr(&cs->cs_queue);

  free(cs);
  *cs_ptr = NULL;
}

void cs_self_pr_add(cs_self *cs, local_id id) {
  if (list_find(cs->act_prs, local_id_pred, &id) == cs->act_prs->end) {
    local_id *id_data = malloc(sizeof(local_id));
    *id_data = id;
    list_insert(cs->act_prs, 0, id_data);
  }
}

void cs_self_pr_rm(cs_self *cs, local_id id) {
  struct list_entry *entry = list_find(cs->act_prs, local_id_pred, &id);
  if (entry != cs->act_prs->end) {
    free(entry->data);
    list_erase(cs->act_prs, entry);
  }
}

void cs_self_queue_push(cs_self *cs, cs_entry entry) {
  cs_entry *inserted = malloc(sizeof(entry));
  inserted->id = entry.id;
  inserted->time = entry.time;

  size_t idx = 0;
  struct list_entry *it;
  for (it = cs->cs_queue->begin; it != cs->cs_queue->end; it = it->next) {
    if (cs_entry_compare(it->data, &entry) >= 0) {
      break;
    }
    idx++;
  }
  list_insert(cs->cs_queue, idx, inserted);
}

int cs_self_queue_empty(cs_self *cs) { return list_empty(cs->cs_queue); }

cs_entry cs_self_queue_top(cs_self *cs) {
  return *(cs_entry *)cs->cs_queue->begin->data;
}

void cs_self_queue_pop(cs_self *cs) {
  list_erase(cs->cs_queue, cs->cs_queue->begin);
}
