#ifndef __LAB4_LAMPORT__H
#define __LAB4_LAMPORT__H

#include "ipc.h"

timestamp_t get_lamport_time(void);

timestamp_t inc_lamport_time(void);

timestamp_t upd_lamport_time(timestamp_t rec_time);

// wrappers that also update lamport time
int send_lamport(void *self, local_id dst, const Message *msg);
int send_multicast_lamport(void * self, const Message * msg);
int receive_lamport(void * self, local_id from, Message * msg);
int receive_any_lamport(void * self, Message * msg);

#endif
