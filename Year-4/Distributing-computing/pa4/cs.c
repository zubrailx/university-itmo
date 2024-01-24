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
//     printf("(%d,%d)", ((cs_entry *)it->data)->time, ((cs_entry *)it->data)->id);
//   }
//   printf("\n");
// }

int request_cs(const void *self) {
  Message req = {.s_header = {0}};
  Message res = {.s_header = {0}};
  cs_self *cs = (cs_self *)self;

  cs_entry cse_cur = {.id = cs->ipc->cur, .time = get_lamport_time() + 1};
  cs_self_queue_push(cs, cse_cur);

  memcpy(req.s_payload, &cse_cur, sizeof(cse_cur));
  message_update_hdr(&req, sizeof(cse_cur), CS_REQUEST);
  send_multicast_lamport(cs->ipc, &req);

  int cnt = list_size(cs->act_prs);
  cs_entry cs_top = cs_self_queue_top(cs);

  while (cs_entry_compare(&cse_cur, &cs_top) != 0 || cnt > 0) {
    // printf("%d:(%d,%d), %d\n", cs->ipc->cur, cse_cur.time, cse_cur.id, cnt);
    // print_queue(cs, cs->ipc->cur);
    receive_any_lamport(cs->ipc, &res);
    switch (res.s_header.s_type) {
    case CS_REQUEST: {
      // update queue
      cs_entry cse_res;
      memcpy(&cse_res, res.s_payload, sizeof(cse_res));
      cs_self_queue_push(cs, cse_res);

      // send reply with current time
      cs_entry cse_now = {.id = cs->ipc->cur, .time = get_lamport_time() + 1};
      memcpy(&req.s_payload, &cse_now, sizeof(cse_now));
      message_update_hdr(&req, sizeof(cse_now), CS_REPLY);
      send_lamport(cs->ipc, cse_res.id, &req);
      break;
    }
    case CS_REPLY: {
      cs_entry cse_res;
      memcpy(&cse_res, res.s_payload, sizeof(cse_res));
      --cnt;
      break;
    }
    case CS_RELEASE: {
      cs_self_queue_pop(cs);
      cs_entry cse_res;
      memcpy(&cse_res, res.s_payload, sizeof(cse_res));
      break;
    }
      // if process is done, no need to wait for it's response,
      // so decrement counter and pop it from pr_list
    case DONE: {
      int time, pr;
      sscanf(res.s_payload, "%d: process %d", &time, &pr);
      cs_self_pr_rm(cs, pr);
      --cnt;
    }
    }

    cs_top = cs_self_queue_top(cs);
  }
  // current pr could answer before entering cs (cnt == -1)
  // or could not (if before) (cnt == 0)
  assert(cnt >= -1);
  return 0;
}

int release_cs(const void *self) {
  Message req = {.s_header = {0}};
  cs_self *cs = (cs_self *)self;

  cs_entry cse_cur = {.id = cs->ipc->cur, .time = get_lamport_time() + 1};
  cs_self_queue_pop(cs);

  memcpy(req.s_payload, &cse_cur, sizeof(cse_cur));
  message_update_hdr(&req, sizeof(cse_cur), CS_RELEASE);
  send_multicast_lamport(cs->ipc, &req);
  return 0;
}
