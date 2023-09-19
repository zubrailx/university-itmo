#include "producer_consumer.h"
#include <pthread.h>
#include <iostream>
#include <iterator>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <vector>

#define INT2VOIDP(i) (void*)(uintptr_t)(i)

struct config config::build(int argc, const char** argv,
                            const std::string& numbers) {
  struct config config;

  std::vector<std::string> args(argv, argv + argc);
  config.n_consumer = std::stoi(args[1]);
  config.ms_max_sleep = std::stoi(args[2]);
  config.is_debug = args.size() == 4 ? args[3].compare("-debug") == 0 : false;

  std::stringstream ss(numbers);
  config.numbers = std::list<int>(std::istream_iterator<int>(ss),
                                  std::istream_iterator<int>());

  return config;
}

static struct { bool is_debug = false; } FLAGS;

static struct {
  pthread_mutex_t tid_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t cons_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_mutex_t task_lock = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t c_prod = PTHREAD_COND_INITIALIZER;
  pthread_cond_t c_cons = PTHREAD_COND_INITIALIZER;
} LOCKS;

struct sh_task {
  int sh_var;
  bool done = false;
  bool full = false;
};

struct producer_args {
  sh_task* task;
  std::list<int> tasks;
};

struct consumer_args {
  sh_task* task;
  int max_st;  // max sleep time
  bool exited = false;
};

struct interruptor_args {
  const std::vector<std::pair<pthread_t, consumer_args>>& ids;
};

static void init_flags(const config& config) {
  FLAGS.is_debug = config.is_debug;
}

// [0; max_val)
static size_t get_rand_val(size_t max) { return max == 0 ? 0 : rand() % max; }

int get_tid() {
  static int tid_seq = 0;
  thread_local std::unique_ptr<int> tid =
      nullptr;  // automatic storage duration
  if (tid == nullptr) {
    pthread_mutex_lock(&LOCKS.tid_lock);
    tid = std::make_unique<int>();
    *tid = ++tid_seq;
    pthread_mutex_unlock(&LOCKS.tid_lock);
  }
  return *tid;
}

static void producer_finish(void* argp) {
  producer_args* arg = (producer_args*)argp;

  pthread_mutex_lock(&LOCKS.task_lock);
  while (arg->task->full) pthread_cond_wait(&LOCKS.c_prod, &LOCKS.task_lock);
  arg->task->done = true;
  pthread_mutex_unlock(&LOCKS.task_lock);
  // info all consumers that task is done
  pthread_cond_broadcast(&LOCKS.c_cons);
}

void* producer_routine(void* argp) {
  producer_args* arg = (producer_args*)argp;

  while (!arg->tasks.empty()) {
    pthread_mutex_lock(&LOCKS.task_lock);

    while (arg->task->full) {
      pthread_cond_wait(&LOCKS.c_prod, &LOCKS.task_lock);
    }
    /* remove element from list */
    arg->task->sh_var = arg->tasks.front();
    arg->tasks.pop_front();
    arg->task->full = true;

    pthread_mutex_unlock(&LOCKS.task_lock);

    pthread_cond_signal(&LOCKS.c_cons);
  }
  producer_finish(argp);
  return nullptr;
}

static int msleep(long msec) {
  struct timespec ts;
  int res;

  if (msec < 0) {
    errno = EINVAL;
    return -1;
  }

  ts.tv_sec = msec / 1000;
  ts.tv_nsec = (msec % 1000) * 1000000;

  do {
    res = nanosleep(&ts, &ts);
  } while (res && errno == EINTR);

  return res;
}

static void consumer_finish(void* argp) {
  consumer_args* arg = (consumer_args*)argp;
  pthread_mutex_lock(&LOCKS.cons_lock);
  arg->exited = true;
  pthread_mutex_unlock(&LOCKS.cons_lock);
}

void* consumer_routine(void* argp) {
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, nullptr);

  consumer_args* arg = (consumer_args*)argp;
  int psum = 0;

  bool do_break = false;

  while (true) {
    pthread_mutex_lock(&LOCKS.task_lock);

    while (!arg->task->done && !arg->task->full) {
      pthread_cond_wait(&LOCKS.c_cons, &LOCKS.task_lock);
    }

    /* handle received response */
    if (!arg->task->done) {
      psum += arg->task->sh_var;
      arg->task->full = false;
      if (FLAGS.is_debug)
        std::cout << "(" << get_tid() << ", " << psum << ")" << std::endl;
    } else {
      do_break = true;
    }

    pthread_mutex_unlock(&LOCKS.task_lock);

    if (do_break) break;

    pthread_cond_signal(&LOCKS.c_prod);
    // sleep thread
    msleep(get_rand_val(arg->max_st));
  }
  // no need to use pthread_cleanup_pop
  consumer_finish(argp);
  return INT2VOIDP(psum);
}

void* interruptor_routine(void* argp) {
  interruptor_args* arg = (interruptor_args*)argp;

  while (true) {
    pthread_mutex_lock(&LOCKS.cons_lock);

    std::vector<pthread_t> running;
    for (const auto& consumer : arg->ids) {
      if (consumer.second.exited == false) {
        running.push_back(consumer.first);
      }
    }

    // exit if no threads running left
    if (running.empty()) {
      pthread_mutex_unlock(&LOCKS.cons_lock);
      break;
    }

    size_t idx = get_rand_val(running.size());
    pthread_cancel(running[idx]);

    pthread_mutex_unlock(&LOCKS.cons_lock);
  }

  pthread_exit(nullptr);
}

// the declaration of run threads can be changed as you like
int run_threads(const struct config& config) {
  init_flags(config);

  sh_task* task = new sh_task;

  struct {
    std::pair<pthread_t, producer_args> producer;
    pthread_t inter_id;
    std::vector<std::pair<pthread_t, consumer_args>> consumers;
  } prog_data;

  // init prog_data
  prog_data.producer.second.task = task;
  prog_data.producer.second.tasks = config.numbers;

  prog_data.consumers =
      std::vector<std::pair<pthread_t, consumer_args>>(config.n_consumer);
  for (auto& consumer : prog_data.consumers) {
    consumer.second.task = task;
    consumer.second.max_st = config.ms_max_sleep;
  }

  // create threads
  pthread_create(&prog_data.producer.first, nullptr, &producer_routine,
                 &prog_data.producer.second);

  for (auto& consumer : prog_data.consumers) {
    pthread_create(&consumer.first, nullptr, &consumer_routine,
                   &consumer.second);
  }

  interruptor_args int_args = {prog_data.consumers};
  pthread_create(&prog_data.inter_id, nullptr, &interruptor_routine, &int_args);

  int result = 0;

  // join threads
  void* end;

  pthread_join(prog_data.producer.first, &end);

  pthread_join(prog_data.inter_id, &end);

  for (const auto& consumer : prog_data.consumers) {
    int cons_res;
    pthread_join(consumer.first, (void**)&cons_res);
    result += cons_res;
  }

  // close resources
  delete task;

  return result;
}
