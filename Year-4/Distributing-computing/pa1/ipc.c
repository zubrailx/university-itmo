#define _BSD_SOURCE

#include "ipc.h"
#include "ipc_self.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int send(void *self, local_id dst, const Message *msg) {
  ipc_self *ipc = self;
  int fd = ipc_self_wfd(ipc, dst);
  int r = write(fd, msg, msg->s_header.s_payload_len + sizeof(MessageHeader));
  return r <= 0 ? -1 : 0;
}

int send_multicast(void *self, const Message *msg) {
  ipc_self *ipc = self;
  int ret = 0;
  for (int i = 0; i < ipc->n; ++i) {
    if (i != ipc->cur) {
      int r = send(self, i, msg);
      if (r == -1) {
        ret = r;
      }
    }
  }
  return ret;
}

// read same size as msg
int receive(void *self, local_id from, Message *msg) {
  ipc_self *ipc = self;
  int fd = ipc_self_rfd(ipc, from);
  int ret = -1;

  if (fd == 0) {
    return ret;
  }

  memset(msg->s_payload, 0, msg->s_header.s_payload_len);

  while (1) {
    int r = read(fd, msg, sizeof(MessageHeader));
    if (r == 0) {
      break;
    } else if (r > 0) {
      r = read(fd, msg->s_payload, msg->s_header.s_payload_len);
      if (r > 0) {
        ret = 0;
      }
      break;
    }
    usleep(1000);
  }
  return ret;
}

int receive_any(void *self, Message *msg) {
  ipc_self *ipc = self;
  int ret = -1;
  int eof_cnt = 0;

  for (int i = 1; i <= ipc->n; ++i) {
    if (eof_cnt > ipc->n) {
      break;
    }

    if (i == ipc->n) { // loop again
      i = 1;
    }

    if (ipc->cur == i) {
      continue;
    }

    int fd = ipc_self_rfd(ipc, i);
    int r = read(fd, msg, sizeof(MessageHeader));

    if (r == 0) {
      ++eof_cnt;
    } else if (r == -1) {
      eof_cnt = 0;
      usleep(1000);
    } else {
      r = read(fd, msg->s_payload, msg->s_header.s_payload_len);
      if (r > 0) {
        ret = 0;
      }
      break;
    }
  }
  return ret;
}
