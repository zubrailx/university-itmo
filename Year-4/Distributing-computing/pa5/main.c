#include "common.h"
#include "cs_self.h"
#include "ipc.h"
#include "ipc_self.h"
#include "lamport.h"
#include "message.h"
#include "pa2345.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static char *prog_name;
static int pr_count;
static int mutexl_flag;

static FILE *events_f, *pipes_f;

void usage(void);
void parse_args(int argc, char *argv[]);

void execute(void);
void execute_child(local_id lpid, ipc_self *ipc);
void execute_parent(local_id lpid, ipc_self *ipc);

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

void execute(void) {
  ipc_self *ipc;

  int is_parent = 1;

  ipc = ipc_self_ctr(pr_count + 1, 0);
  write_pipes_f(ipc);

  for (local_id i = 1; i <= pr_count; ++i) {
    int fr = fork();
    if (fr == 0) {
      is_parent = 0;
      ipc_self_set_cur(ipc, i);
      ipc_self_pipe_link(ipc);
      execute_child(i, ipc);
      break;
    }
  }

  if (is_parent) {
    local_id parent_id = 0;
    ipc_self_set_cur(ipc, parent_id);
    ipc_self_pipe_link(ipc);
    execute_parent(parent_id, ipc);
  }

  ipc_self_dtr(&ipc);
}

void execute_parent(local_id lpid, ipc_self *ipc) {
  int wpid;
  int status = 0;

  while ((wpid = wait(&status)) > 0)
    ;
}

void execute_child(local_id lpid, ipc_self *ipc) {
  size_t l;
  Message req = {.s_header = {0}};
  Message res = {.s_header = {0}};

  ipc_self_pipe_link(ipc);

  // NOTE: maybe need to use balance
  l = sprintf(req.s_payload, log_started_fmt, get_lamport_time(), lpid,
              getpid(), getppid(), 0);
  write_out_events_f(req.s_payload, l);

  message_update_hdr(&req, l, STARTED);
  send_multicast_lamport(ipc, &req);

  for (local_id i = 1; i <= pr_count; ++i) {
    if (i != lpid) {
      receive_lamport(ipc, i, &res);
      if (res.s_header.s_type != STARTED) {
        write_out_unexpected_type(i, "STARTED", res.s_header.s_type);
        exit(-1);
      }
    }
  }

  l = sprintf(req.s_payload, log_received_all_started_fmt, get_lamport_time(),
              lpid);
  write_out_events_f(req.s_payload, l);

  // add process ids that are listened
  cs_self *cs = cs_self_ctr(ipc);
  for (local_id i = 1; i <= pr_count; ++i) {
    if (i != lpid) {
      cs_self_pr_add(cs, i);
    }
  }

  const int iters = lpid * 5;
  for (int i = 1; i <= iters; ++i) {
    if (mutexl_flag) {
      request_cs(cs);
    }

    l = sprintf(req.s_payload, log_loop_operation_fmt, lpid, i, iters);
    print(req.s_payload);

    if (mutexl_flag) {
      release_cs(cs);
    }
  }

  l = sprintf(req.s_payload, log_done_fmt, get_lamport_time(), lpid, 0);

  message_update_hdr(&req, l, DONE);
  send_multicast_lamport(ipc, &req);

  while (!list_empty(cs->pr_rep_l)) {
    receive_any_lamport(ipc, &res);
    if (res.s_header.s_type == DONE) {
      int time, pr;
      sscanf(res.s_payload, "%d: process %d", &time, &pr);
      cs_self_pr_rm(cs, pr);
    }
  }

  l = sprintf(req.s_payload, log_received_all_done_fmt, get_lamport_time(),
              lpid);
  write_out_events_f(req.s_payload, l);

  cs_self_dtr(&cs);
}

void usage(void) { printf("Usage: \nprog -p <pr_count> [--mutexl]\n"); }

void parse_args(int argc, char *argv[]) {
  int c;
  prog_name = argv[0];

  while (1) {
    static struct option long_options[] = {
        {"mutexl", no_argument, &mutexl_flag, 1},
    };

    int option_index = 0;

    c = getopt_long(argc, argv, "p:h", long_options, &option_index);

    if (c == EOF) {
      break;
    }
    switch (c) {
    case 0:
      // if this option only sets flag
      if (long_options[option_index].flag != 0)
        break;
      break;
    case 'p':
      pr_count = strtol(optarg, NULL, 10);
      break;
    case 'h':
      usage();
      break;
    default:
      usage();
      exit(-1);
      break;
    }
  }

  argc -= optind;
  argv += optind;

  if (pr_count <= 0) {
    printf("p <= 0\n");
    exit(-1);
  }

  if (argc != 0) {
    usage();
    exit(-1);
  }
}

int main(int argc, char *argv[]) {
  parse_args(argc, argv);

  events_f = fopen(events_log, "w");
  pipes_f = fopen(pipes_log, "w");

  execute();

  fclose(pipes_f);
  fclose(events_f);

  return 0;
}
