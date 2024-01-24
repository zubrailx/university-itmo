#ifndef __IFMO_DISTRIBUTED_CLASS_TRANSFER__H
#define __IFMO_DISTRIBUTED_CLASS_TRANSFER__H

#include "ipc.h"
#include "banking.h"

enum transfer_dir {
  TRANSFER_TO_SRC,
  TRANSFER_TO_DST,
};

struct transfer_data {
  struct ipc_self *ipc_self;
  Message * msg_buf;
};

struct transfer_payload {
  enum transfer_dir dir;
  local_id src;
  local_id dst;
  balance_t amount;
};

enum transfer_dir get_transfer_dir(void *parent_data);



#endif
