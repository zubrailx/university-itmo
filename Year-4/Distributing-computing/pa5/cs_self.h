#ifndef __LAB4_CS_SELF__H
#define __LAB4_CS_SELF__H

#include "ipc.h"
#include "ipc_self.h"
#include "util.h"

typedef struct cs_req_entry {
  timestamp_t time;
  local_id id;
} cs_req_entry;

typedef struct cs_pr_entry {
  local_id id;
  uint8_t is_def;
  uint8_t is_replied;
} cs_pr_entry;

typedef struct cs_self {
  ipc_self *ipc;
  struct list *pr_rep_l; // processes to be replied list (cs_pr_entry)
} cs_self;

// entry of list of deferred replies

cs_self *cs_self_ctr(ipc_self *ipc);
void cs_self_dtr(cs_self **cs);

// add process to the list of listened
void cs_self_pr_add(cs_self *cs, local_id id);
void cs_self_pr_rm(cs_self *cs, local_id id);
cs_pr_entry *cs_self_pr_get(cs_self *cs, local_id id);

int cs_req_compare(cs_req_entry *lhs, cs_req_entry *rhs);

#endif
