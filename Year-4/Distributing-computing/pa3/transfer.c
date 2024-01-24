#include "transfer.h"
#include "ipc.h"
#include "lamport.h"

enum transfer_dir get_transfer_dir(void *parent_data) {
  TransferOrder *order = parent_data;
  return order->s_amount > 0 ? TRANSFER_TO_SRC : TRANSFER_TO_DST;
}

void transfer(void *parent_data, local_id src, local_id dst, balance_t amount) {
  struct transfer_data *td = parent_data;
  Message *msg = td->msg_buf;

  msg->s_header.s_magic = MESSAGE_MAGIC;
  msg->s_header.s_type = TRANSFER;
  msg->s_header.s_payload_len = sizeof(TransferOrder);

  TransferOrder *trhdr = (TransferOrder *)msg->s_payload;
  trhdr->s_src = src;
  trhdr->s_dst = dst;
  trhdr->s_amount = amount;

  send_lamport(td->ipc_self, src, msg);
  receive_lamport(td->ipc_self, dst, msg);
}
