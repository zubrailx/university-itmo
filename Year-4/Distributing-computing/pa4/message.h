#ifndef __LAB4_MESSAGE__H
#define __LAB4_MESSAGE__H

#include "ipc.h"

void message_update_hdr(Message *msg, uint16_t payload_len, uint16_t type);

#endif
