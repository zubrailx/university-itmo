#include "common.h"
#include "ipc.h"
#include "ipc_self.h"
#include "pa1.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

Message *ctr_message(size_t payload, int16_t type, timestamp_t localtime);
void dtr_message(Message **msg_p);
int execute(local_id pid, ipc_self *ipc);

char *pr_name;
int pr_count;

FILE *events_f, *pipes_f;

void usage(void) {
  printf("Usage: %s p <pr_count>\n", pr_name);
  exit(-1);
}

int main(int argc, char *argv[]) {
  int i;

  pr_name = argv[0];

  while ((i = getopt(argc, argv, "p:h")) != EOF) {
    switch (i) {
    case 'p':
      pr_count = strtol(optarg, NULL, 10);
      break;
    case 'h':
      usage();
      break;
    }
  }

  if (pr_count < 0) {
    printf("p < 0\n");
    exit(-1);
  }

  argc -= optind;
  argv += optind;

  if (argc < 0 || argc > 1) {
    usage();
    exit(-1);
  }

  events_f = fopen(events_log, "w");
  pipes_f = fopen(pipes_log, "w");

  ipc_self *ipc = ipc_self_ctr(pr_count + 1, 0);

  for (int i = 0; i < ipc->n; ++i) {
    for (int j = 0; j < ipc->n; ++j) {
      if (i == j)
        continue;
      int *pipe = ipc->pipes + 2 * (i * ipc->n + j);
      fprintf(pipes_f, "%d/%d\n", pipe[0], pipe[1]);
    }
  }
  fflush(pipes_f);

  int is_parent = 1;

  for (local_id i = 1; i <= pr_count; ++i) {
    int fr = fork();
    if (fr == 0) {
      is_parent = 0;
      ipc_self_set_cur(ipc, i);
      execute(i, ipc);
      break;
    } else if (fr < 0) {
      perror("fork");
      exit(-1);
    }
  }

  if (is_parent) {
    Message *msg = ctr_message(256, 0, 0);

    ipc_self_pipe_link(ipc);

    // receive started
    for (local_id i = 1; i <= pr_count; ++i) {
      receive_any(ipc, msg);
    }

    // receive done
    for (local_id i = 1; i <= pr_count; ++i) {
      receive_any(ipc, msg);
    }

    int wpid;
    int status = 0;

    while ((wpid = wait(&status)) > 0)
      ;

    dtr_message(&msg);
  }

  ipc_self_dtr(&ipc);
  fclose(pipes_f);
  fclose(events_f);

  return 0;
}

void write_out_events_f(char *buf, size_t size) {
  fwrite(buf, sizeof(char), size, events_f);
  printf("%s", buf);
}

Message *ctr_message(size_t payload, int16_t type, timestamp_t localtime) {
  Message *msg = (Message *)calloc(1, sizeof(MessageHeader) + payload);
  msg->s_header.s_magic = MESSAGE_MAGIC;
  msg->s_header.s_payload_len = payload;
  msg->s_header.s_type = type;
  msg->s_header.s_local_time = localtime;
  return msg;
}

void dtr_message(Message **msg_p) {
  if (msg_p != NULL) {
    free(*msg_p);
    *msg_p = NULL;
  }
}

int execute(local_id lpid, ipc_self *ipc) {
  int ret = 0;
  size_t p_size = 256;
  char buf[p_size];
  size_t l;

  Message *req = ctr_message(p_size, 0, 0);
  Message *res = ctr_message(p_size, 0, 0);

  ipc_self_pipe_link(ipc);

  l = sprintf(buf, log_started_fmt, lpid, getpid(), getppid());
  write_out_events_f(buf, l);

  req->s_header.s_type = STARTED;
  memcpy(req->s_payload, buf, l);
  req->s_header.s_payload_len = l;
  send_multicast(ipc, req);

  for (local_id i = pr_count; i > 0; --i) {
    if (i != lpid) {
      receive(ipc, i, res);
      if (res->s_header.s_type != STARTED) {
        printf("unexpected s_type from %d, expected STARTED, received %d\n", i,
               res->s_header.s_type);
        ret = -1;
        goto free;
      }
    }
  }

  l = sprintf(buf, log_received_all_started_fmt, lpid);
  write_out_events_f(buf, l);

  l = sprintf(buf, log_done_fmt, lpid);
  write_out_events_f(buf, l);

  req->s_header.s_type = DONE;
  memcpy(req->s_payload, buf, l);
  req->s_header.s_payload_len = l;
  send_multicast(ipc, req);

  for (local_id i = pr_count; i > 0; --i) {
    if (i != lpid) {
      receive_any(ipc, res);
      if (res->s_header.s_type != DONE) {
        printf("unexpected s_type from %d, expected DONE, received %d\n", i,
               res->s_header.s_type);
        ret = -1;
        goto free;
      }
    }
  }

  l = sprintf(buf, log_received_all_done_fmt, lpid);
  write_out_events_f(buf, l);

free:
  dtr_message(&req);
  dtr_message(&res);

  return ret;
}
