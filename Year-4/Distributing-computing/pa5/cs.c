#include "cs_self.h"
#include "ipc.h"
#include "lamport.h"
#include "message.h"
#include "pa2345.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// static void print_queue(cs_self *cs, local_id lpid) {
//   printf("%d:", lpid);
//   for (struct list_entry *it = cs->cs_queue->begin; it != cs->cs_queue->end;
//        it = it->next) {
//     printf("(%d,%d)", ((cs_entry *)it->data)->time, ((cs_entry
//     *)it->data)->id);
//   }
//   printf("\n");
// }
void print_cs_pr(cs_self *cs) {
  struct list_entry *it;
  for (it = cs->pr_rep_l->begin; it != cs->pr_rep_l->end; it = it->next) {
    struct cs_pr_entry *entry = it->data;
    printf("(%d,%d)", entry->id, entry->is_def);
  }
  printf("\n");
}

int cs_self_pr_is_replied_all(cs_self *cs) {
  struct list_entry *it;
  uint8_t v = 1;
  for (it = cs->pr_rep_l->begin; it != cs->pr_rep_l->end; it = it->next) {
    v &= ((cs_pr_entry *)it->data)->is_replied;
  }
  return v;
}

int request_cs(const void *self) {
  Message req = {.s_header = {0}};
  Message res = {.s_header = {0}};
  cs_self *cs = (cs_self *)self;

  cs_req_entry cse_cur = {.id = cs->ipc->cur, .time = get_lamport_time() + 1};

  memcpy(req.s_payload, &cse_cur, sizeof(cse_cur));
  message_update_hdr(&req, sizeof(cse_cur), CS_REQUEST);
  send_multicast_lamport(cs->ipc, &req);

  while (!cs_self_pr_is_replied_all(cs)) {
    receive_any_lamport(cs->ipc, &res);

    switch (res.s_header.s_type) {
    case CS_REQUEST: {
      cs_req_entry cse_res;
      memcpy(&cse_res, res.s_payload, sizeof(cse_res));

      if (cs_req_compare(&cse_cur, &cse_res) > 0) {
        cs_req_entry cse_now = {.id = cs->ipc->cur,
                                .time = get_lamport_time() + 1};
        memcpy(&req.s_payload, &cse_now, sizeof(cse_now));
        message_update_hdr(&req, sizeof(cse_now), CS_REPLY);
        send_lamport(cs->ipc, cse_res.id, &req);
      } else {
        cs_self_pr_get(cs, cse_res.id)->is_def = 1;
      }

      break;
    }
    case CS_REPLY: {
      cs_req_entry cse_res;
      memcpy(&cse_res, res.s_payload, sizeof(cse_res));
      cs_self_pr_get(cs, cse_res.id)->is_replied = 1;
      break;
    }
    case DONE: {
      int time, pr;
      sscanf(res.s_payload, "%d: process %d", &time, &pr);
      cs_self_pr_rm(cs, pr);
    }
    }
  }
  return 0;
}

int release_cs(const void *self) {
  Message req = {.s_header = {0}};
  cs_self *cs = (cs_self *)self;

  cs_req_entry cse_cur = {.id = cs->ipc->cur, .time = get_lamport_time() + 1};

  memcpy(req.s_payload, &cse_cur, sizeof(cse_cur));
  message_update_hdr(&req, sizeof(cse_cur), CS_REPLY);

  struct list_entry *it;
  for (it = cs->pr_rep_l->begin; it != cs->pr_rep_l->end; it = it->next) {
    cs_pr_entry *entry = it->data;
    entry->is_replied = 0;
    if (entry->is_def != 0) {
      send(cs->ipc, entry->id, &req);
      entry->is_def = 0;
    }
  }

  return 0;
}
