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
  int ret = write(fd, msg, msg->s_header.s_payload_len + sizeof(MessageHeader));
  ret = ret <= 0 ? -1 : 0;
  if (ret != 0) {
    perror("send");
  }
  return ret;
}

int send_multicast(void *self, const Message *msg) {
  ipc_self *ipc = self;
  int ret = 0;
  int i;
  for (i = 0; i < ipc->n; ++i) {
    if (i != ipc->cur) {
      int r = send(self, i, msg);
      if (r == -1) {
        char buf[256];
        sprintf(buf, "send_multicast for %d", i);
        perror(buf);
        ret = r;
      }
    }
  }
  return ret;
}

int receive(void *self, local_id from, Message *msg) {
  ipc_self *ipc = self;
  int fd = ipc_self_rfd(ipc, from);
  int ret = -1;

  if (fd == 0) {
    printf("fd == 0\n");
    return ret;
  }

  int r;
  do {
    r = read(fd, msg, sizeof(MessageHeader));
    if (r == -1) {
      sleep(0);
    }
  } while (r == -1);

  if (r > 0) {
    do {
      if (msg->s_header.s_payload_len) {
        r = read(fd, msg->s_payload, msg->s_header.s_payload_len);
      }
      if (r > 0) {
        ret = 0;
      } else if (r == -1) {
        sleep(0);
      }
    } while (r == -1);
  }

  if (r == 0) {
    printf("receive %d from %d: cause eof\n", ipc->cur, from);
  }

  return ret;
}

int receive_any(void *self, Message *msg) {
  ipc_self *ipc = self;
  int ret = -1;
  int eof_cnt = 0;

  int r;
  int fd;

  for (int i = 0; i <= ipc->n; ++i) {
    if (eof_cnt > ipc->n) {
      break;
    }

    if (i == ipc->n) { // loop again
      i = 0;
    }

    if (ipc->cur == i) {
      continue;
    }

    fd = ipc_self_rfd(ipc, i);
    r = read(fd, msg, sizeof(MessageHeader));

    if (r == 0) {
      ++eof_cnt;
    } else if (r == -1) {
      eof_cnt = 0;
      sleep(0);
    } else {
      break;
    }
  }

  if (r > 0) {
    do {
      if (msg->s_header.s_payload_len) {
        r = read(fd, msg->s_payload, msg->s_header.s_payload_len);
      }
      if (r > 0) {
        ret = 0;
      } else if (r == -1) {
        sleep(0);
      }
    } while (r == -1);
  }

  if (r == 0) {
    printf("receive_any %d: cause eof\n", ipc->cur);
  }

  return ret;
}
