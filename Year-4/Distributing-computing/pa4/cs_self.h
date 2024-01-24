#ifndef __LAB4_CS_SELF__H
#define __LAB4_CS_SELF__H

#include "ipc.h"
#include "ipc_self.h"
#include "util.h"

typedef struct cs_entry {
  timestamp_t time;
  local_id id;
} cs_entry;

typedef struct cs_self {
  ipc_self *ipc;
  struct list *cs_queue;
  struct list *act_prs; // active process list
} cs_self;

cs_self *cs_self_ctr(ipc_self *ipc);
void cs_self_dtr(cs_self **cs);

// add process to the list of listened
void cs_self_pr_add(cs_self *cs, local_id id);
void cs_self_pr_rm(cs_self *cs, local_id id);

void cs_self_queue_push(cs_self *cs, cs_entry entry);
int cs_self_queue_empty(cs_self *cs);
cs_entry cs_self_queue_top(cs_self *cs);
void cs_self_queue_pop(cs_self *cs);

int cs_entry_compare(cs_entry *lhs, cs_entry *rhs);

#endif
