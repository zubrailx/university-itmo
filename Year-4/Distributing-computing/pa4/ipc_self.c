#define _GNU_SOURCE

#include "ipc_self.h"
#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

ipc_self *ipc_self_ctr(size_t n, local_id cur) {
  int *pipes = calloc(1, sizeof(int) * n * n * 2);
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      if (i != j) {
        int *pip = pipes + 2 * (i * n + j);
        pipe2(pip, O_NONBLOCK);
      }
    }
  }
  ipc_self *ipc = malloc(sizeof(ipc_self));
  ipc->n = n;
  ipc->pipes = pipes;
  ipc->cur = 0;
  return ipc;
}

void ipc_self_dtr(ipc_self **ipc_p) {
  if (ipc_p != NULL) {
    free((*ipc_p)->pipes);
    free(*ipc_p);
    *ipc_p = NULL;
  }
}

void ipc_self_set_cur(ipc_self *ipc, local_id cur) { ipc->cur = cur; }

int *get_pipe(ipc_self *ipc, local_id i, local_id j) {
  return ipc->pipes + 2 * (ipc->n * i + j);
}

// first column -> read, close unused fds
void ipc_self_pipe_link(ipc_self *ipc) {
  for (int i = 0; i < ipc->n; ++i) {
    for (int j = 0; j < ipc->n; ++j) {
      int *pipe = get_pipe(ipc, i, j);
      if (i == j) {
        continue;
      } else if (i == ipc->cur) {
        close(pipe[0]);
      } else if (j == ipc->cur) {
        close(pipe[1]);
      } else {
        close(pipe[0]);
        close(pipe[1]);
      }
    }
  }
}

int ipc_self_wfd(ipc_self *ipc, local_id to) {
  return get_pipe(ipc, ipc->cur, to)[1];
}

int ipc_self_rfd(ipc_self *ipc, local_id from) {
  return get_pipe(ipc, from, ipc->cur)[0];
}
