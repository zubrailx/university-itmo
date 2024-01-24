#include "message.h"

void message_update_hdr(Message *msg, uint16_t payload_len, uint16_t type) {
  msg->s_header.s_magic = MESSAGE_MAGIC;
  msg->s_header.s_payload_len = payload_len;
  msg->s_header.s_type = type;
}

