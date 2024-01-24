#include "transfer.h"

enum transfer_dir get_transfer_dir(void *parent_data) {
  struct transfer_payload *tp = parent_data;
  return tp->dir;
}

void transfer(void *parent_data, local_id src, local_id dst, balance_t amount) {
  struct transfer_data *td = parent_data;
  Message *msg = td->msg_buf;

  msg->s_header.s_type = TRANSFER;
  msg->s_header.s_payload_len = sizeof(struct transfer_payload);

  struct transfer_payload *trhdr = (struct transfer_payload *)msg->s_payload;
  trhdr->dir = TRANSFER_TO_SRC;
  trhdr->src = src;
  trhdr->dst = dst;
  trhdr->amount = amount;

  send(td->ipc_self, src, msg);
  receive(td->ipc_self, dst, msg);
}
