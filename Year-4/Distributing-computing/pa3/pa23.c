#include "banking.h"
#include "common.h"
#include "ipc.h"
#include "ipc_self.h"
#include "lamport.h"
#include "pa2345.h"
#include "transfer.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

char *pr_name;
int c_count;
int *balances;

FILE *events_f, *pipes_f;

void usage(void) {
  printf("Usage: %s p <pr_count> [S...]\n", pr_name);
  printf("          #[S...] == <pr_count>\n");
  exit(-1);
}

void parse_args(int argc, char *argv[]) {
  int i;

  pr_name = argv[0];

  while ((i = getopt(argc, argv, "p:h")) != EOF) {
    switch (i) {
    case 'p':
      c_count = strtol(optarg, NULL, 10);
      break;
    case 'h':
      usage();
      break;
    }
  }

  if (c_count <= 0) {
    printf("p <= 0\n");
    exit(-1);
  }

  argc -= optind;
  argv += optind;

  if (argc < c_count || argc > c_count + 1) {
    usage();
    exit(-1);
  }

  balances = malloc(sizeof(int) * c_count);

  for (int i = 0; i < c_count; ++i, ++argv, --argc) {
    balances[i] = strtol(argv[0], NULL, 10);
  }
}

void write_pipes_f(ipc_self *ipc) {
  for (int i = 0; i < ipc->n; ++i) {
    for (int j = 0; j < ipc->n; ++j) {
      if (i == j)
        continue;
      int *pipe = ipc->pipes + 2 * (i * ipc->n + j);
      fprintf(pipes_f, "%d/%d\n", pipe[0], pipe[1]);
    }
  }
  fflush(pipes_f);
}

void write_out_events_f(char *buf, size_t size) {
  fwrite(buf, sizeof(char), size, events_f);
  printf("%s", buf);
}

void write_out_unexpected_type(local_id lpid, char *exp, int16_t rec) {
  printf("unexpected s_type from %d, expected %s, received %d\n", lpid, exp,
         rec);
}

void normalize_all_history(AllHistory *ah) {
  uint8_t max_time = 0;
  for (int i = 0; i < ah->s_history_len; ++i) {
    BalanceHistory *bh = ah->s_history + i;
    uint8_t bhl = bh->s_history_len;
    if (bhl != 0 && bh->s_history[bhl - 1].s_time > max_time) {
      max_time = bh->s_history[bhl - 1].s_time;
    }
  }

  for (int i = 0; i < ah->s_history_len; ++i) {
    BalanceHistory *bh = ah->s_history + i;
    for (int j = bh->s_history_len - 1, k = max_time; k >= 0 && j >= 0; --k) {
      bh->s_history[k] = bh->s_history[j];
      bh->s_history[k].s_time = k;
      if (k <= bh->s_history[j].s_time) {
        --j;
      }
    }
    bh->s_history_len = max_time + 1;
  }
}

void message_update_hdr(Message *msg, uint16_t payload_len, uint16_t type) {
  msg->s_header.s_magic = MESSAGE_MAGIC;
  msg->s_header.s_payload_len = payload_len;
  msg->s_header.s_type = type;
}

void execute_parent(ipc_self *ipc) {
  Message msg = {.s_header = {0}};
  AllHistory ah = {0};

  ipc_self_pipe_link(ipc);

  for (int i = 1; i <= c_count; ++i) {
    int r = receive_lamport(ipc, i, &msg);
    if (r) {
      printf("can't receive message");
      exit(-1);
    }
    if (msg.s_header.s_type != STARTED) {
      write_out_unexpected_type(i, "STARTED", msg.s_header.s_type);
    }
  }

  struct transfer_data td = {.ipc_self = ipc, .msg_buf = &msg};
  bank_robbery(&td, c_count);

  message_update_hdr(&msg, 0, STOP);
  send_multicast_lamport(ipc, &msg);

  for (int i = 1; i <= c_count; ++i) {
    receive_lamport(ipc, i, &msg);
    if (msg.s_header.s_type != DONE) {
      write_out_unexpected_type(i, "DONE", msg.s_header.s_type);
    }
  }

  for (int i = 1; i <= c_count; ++i) {
    receive_lamport(ipc, i, &msg);
    if (msg.s_header.s_type != BALANCE_HISTORY) {
      write_out_unexpected_type(i, "BALANCE_HISTORY", msg.s_header.s_type);
    }

    memcpy(ah.s_history + ah.s_history_len++, msg.s_payload,
           sizeof(BalanceHistory));
  }

  normalize_all_history(&ah);
  print_history(&ah);

  int wpid, status;
  while ((wpid = wait(&status)) > 0)
    ;
}

void balance_state_update(BalanceState *bs, balance_t balance) {
  bs->s_time = get_lamport_time();
  bs->s_balance = balance;
}

void balance_history_update(BalanceHistory *bh, const BalanceState *bs) {
  if (bh->s_history_len == 0) {
    bh->s_history[bh->s_history_len] = *bs;
    bh->s_history_len++;
  } else {
    while (bh->s_history[bh->s_history_len - 1].s_time != bs->s_time) {
      bh->s_history[bh->s_history_len] = bh->s_history[bh->s_history_len - 1];
      bh->s_history[bh->s_history_len].s_time += 1;
      bh->s_history_len++;
    }
    bh->s_history[bh->s_history_len - 1] = *bs;
  }
}

void balance_update(BalanceHistory *bh, BalanceState *bs, balance_t balance) {
  balance_state_update(bs, balance);
  balance_history_update(bh, bs);
}

// inc pending value for each state from begin to end
void balance_pending_update(BalanceHistory *bh, timestamp_t begin,
                            timestamp_t end, balance_t pending) {
  for (int i = 0; i < bh->s_history_len; ++i) {
    BalanceState *bs = bh->s_history + i;
    if (bs->s_time >= begin && bs->s_time < end) {
      bs->s_balance_pending_in += pending;
    }
  }
}

void execute_child(local_id lpid, ipc_self *ipc) {
  Message msg = {.s_header = {0}};
  BalanceState bs = {0};
  BalanceHistory bh = {.s_id = lpid};
  int l;

  ipc_self_set_cur(ipc, lpid);
  ipc_self_pipe_link(ipc);

  balance_update(&bh, &bs, balances[lpid - 1]);

  // send STARTED
  l = sprintf(msg.s_payload, log_started_fmt, get_lamport_time(), lpid,
              getpid(), getppid(), bs.s_balance);
  message_update_hdr(&msg, l, STARTED);
  send_multicast_lamport(ipc, &msg);
  write_out_events_f(msg.s_payload, l);

  for (local_id i = 1; i <= c_count; ++i) {
    if (i != lpid) {
      receive_lamport(ipc, i, &msg);
      if (msg.s_header.s_type != STARTED) {
        write_out_unexpected_type(i, "STARTED", msg.s_header.s_type);
      }
    }
  }

  l = sprintf(msg.s_payload, log_received_all_started_fmt, bs.s_time, lpid);
  write_out_events_f(msg.s_payload, l);

  enum transfer_cap {
    SENDABLE = 0x01,
    RECEIVABLE = 0x02,
  };

  int caps = SENDABLE | RECEIVABLE;
  int c_active = c_count;

  while (caps) {
    if ((caps & RECEIVABLE) && c_active <= 0) {
      caps &= ~RECEIVABLE;
      continue;
    }

    receive_any_lamport(ipc, &msg);
    timestamp_t pending_in_time = msg.s_header.s_local_time - 1;

    switch (msg.s_header.s_type) {
    case STOP:
      caps &= ~SENDABLE;
      --c_active;
      // send DONE
      l = sprintf(msg.s_payload, log_done_fmt, get_lamport_time(), lpid,
                  bs.s_balance);
      message_update_hdr(&msg, l, DONE);
      send_multicast_lamport(ipc, &msg);
      write_out_events_f(msg.s_payload, l);
      break;
    case DONE:
      --c_active;
      break;
    // when sending -> balance is positive
    // when receiving -> balance is negative
    case TRANSFER:
      if (get_transfer_dir(msg.s_payload) == TRANSFER_TO_SRC) {
        TransferOrder *order = (TransferOrder *)msg.s_payload;

        // NOTE: balance update has same timestamp as when sending
        balance_update(&bh, &bs, bs.s_balance - order->s_amount);

        order->s_amount = -1 * order->s_amount;
        send_lamport(ipc, order->s_dst, &msg);

        l = sprintf(msg.s_payload, log_transfer_out_fmt, get_lamport_time(),
                    order->s_src, -order->s_amount, order->s_dst);
        write_out_events_f(msg.s_payload, l);
      } else {
        TransferOrder *order = (TransferOrder *)msg.s_payload;
        order->s_amount = -1 * order->s_amount;

        balance_update(&bh, &bs, bs.s_balance + order->s_amount);
        balance_pending_update(&bh, pending_in_time, get_lamport_time(),
                               order->s_amount);

        l = sprintf(msg.s_payload, log_transfer_in_fmt, get_lamport_time(),
                    order->s_dst, order->s_amount, order->s_src);
        write_out_events_f(msg.s_payload, l);

        msg.s_header.s_type = ACK;
        msg.s_header.s_payload_len = 0;
        memset(order, 0, sizeof(TransferOrder));

        send_lamport(ipc, 0, &msg);
      }
    }
  }

  l = sprintf(msg.s_payload, log_received_all_done_fmt, get_lamport_time(),
              lpid);
  write_out_events_f(msg.s_payload, l);

  // send BALANCE_HISTORY to K
  memcpy(msg.s_payload, &bh, sizeof(bh));
  message_update_hdr(&msg, sizeof(bh), BALANCE_HISTORY);
  send_lamport(ipc, 0, &msg);
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);

  events_f = fopen(events_log, "w");
  pipes_f = fopen(pipes_log, "w");

  ipc_self *ipc = ipc_self_ctr(c_count + 1, 0);
  write_pipes_f(ipc);

  int is_parent = 1;

  for (int i = 1; i <= c_count; ++i) {
    int fr = fork();
    if (fr == 0) {
      is_parent = 0;
      execute_child(i, ipc);
      break;
    } else if (fr < 0) {
      perror("fork");
      exit(-1);
    }
  }

  if (is_parent) {
    execute_parent(ipc);
  }

  ipc_self_dtr(&ipc);

  fclose(pipes_f);
  fclose(events_f);
  free(balances);

  return 0;
}
