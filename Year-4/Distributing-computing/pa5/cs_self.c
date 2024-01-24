#include "cs_self.h"
#include <stdio.h>
#include <stdlib.h>

static int cs_pr_local_id_pred(void *lhs, void *rhs) {
  local_id *lhs1 = lhs;
  cs_pr_entry *rhs1 = rhs;
  return *lhs1 == rhs1->id;
}

// -1 - less, 0 - eq, 1 - greater
int cs_req_compare(cs_req_entry *lhs, cs_req_entry *rhs) {
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
  cs->pr_rep_l = list_ctr();
  return cs;
}

void cs_self_dtr(cs_self **cs_ptr) {
  cs_self *cs = *cs_ptr;

  for (struct list_entry *it = cs->pr_rep_l->begin; it != cs->pr_rep_l->end;
       it = it->next) {
    free(it->data);
  }
  list_dtr(&cs->pr_rep_l);

  free(cs);
  *cs_ptr = NULL;
}

void cs_self_pr_add(cs_self *cs, local_id id) {
  if (list_find(cs->pr_rep_l, cs_pr_local_id_pred, &id) == cs->pr_rep_l->end) {
    struct cs_pr_entry *pr_data =
        (struct cs_pr_entry *)malloc(sizeof(cs_pr_entry));
    pr_data->id = id;
    pr_data->is_def = 0;
    pr_data->is_replied = 0;
    list_insert(cs->pr_rep_l, 0, pr_data);
  }
}

void cs_self_pr_rm(cs_self *cs, local_id id) {
  struct list_entry *entry = list_find(cs->pr_rep_l, cs_pr_local_id_pred, &id);
  if (entry != cs->pr_rep_l->end) {
    free(entry->data);
    list_erase(cs->pr_rep_l, entry);
  }
}

cs_pr_entry *cs_self_pr_get(cs_self *cs, local_id id) {
  return list_find(cs->pr_rep_l, cs_pr_local_id_pred, &id)->data;
}
