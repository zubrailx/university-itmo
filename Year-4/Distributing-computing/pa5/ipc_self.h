#ifndef __LAB4_IPC_SELF__H
#define __LAB4_IPC_SELF__H

#include "ipc.h"
#include <stddef.h>

typedef struct ipc_self {
  int *pipes;
  int n;
  local_id cur;
} ipc_self;

ipc_self *ipc_self_ctr(size_t n, local_id cur);
void ipc_self_dtr(ipc_self **ipc_p);
void ipc_self_set_cur(ipc_self *ipc, local_id cur);
void ipc_self_pipe_link(ipc_self *ipc);
int ipc_self_wfd(ipc_self *ipc, local_id to);
int ipc_self_rfd(ipc_self *ipc, local_id from);

#endif
