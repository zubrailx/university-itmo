#include "banking.h"
#include "ipc.h"

static timestamp_t lamport_time = 0;

timestamp_t get_lamport_time() { return lamport_time; }

timestamp_t inc_lamport_time() { return ++lamport_time; }

timestamp_t upd_lamport_time(timestamp_t rec_time) {
  if (rec_time > lamport_time) {
    lamport_time = rec_time;
  }
  return lamport_time;
}

int send_lamport(void *self, local_id dst, Message *msg) {
  msg->s_header.s_local_time = inc_lamport_time();
  return send(self, dst, msg);
}

int send_multicast_lamport(void *self, Message *msg) {
  msg->s_header.s_local_time = inc_lamport_time();
  return send_multicast(self, msg);
}

int receive_lamport(void *self, local_id from, Message *msg) {
  int ret = receive(self, from, msg);
  if (!ret) {
    upd_lamport_time(msg->s_header.s_local_time);
    inc_lamport_time();
  }
  return ret;
}

int receive_any_lamport(void *self, Message *msg) {
  int ret = receive_any(self, msg);
  if (!ret) {
    upd_lamport_time(msg->s_header.s_local_time);
    inc_lamport_time();
  }
  return ret;
}
