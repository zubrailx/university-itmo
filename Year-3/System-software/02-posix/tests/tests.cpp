#include <chrono>
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <producer_consumer.h>
#include <numeric>
#include "doctest.h"

TEST_CASE("config_build") {
  int argc = 4;
  const char **argv = (const char **)malloc(sizeof(char *) * argc);
  argv[0] = "";
  argv[1] = "5";
  argv[2] = "1000";
  argv[3] = "-debug";
  std::string numbers("5 10 15");
  auto config = config::build(argc, argv, numbers);

  struct config ex_config {
    true, 1000, 5, std::list<int>{5, 10, 15},
  };

  CHECK(std::equal(ex_config.numbers.begin(), ex_config.numbers.end(),
                   config.numbers.begin(), config.numbers.end()));
  CHECK_EQ(ex_config.is_debug, config.is_debug);
  CHECK_EQ(ex_config.n_consumer, config.n_consumer);
  CHECK_EQ(ex_config.ms_max_sleep, config.ms_max_sleep);
}

TEST_CASE("gettid") {
  int n_threads = 10;
  for (int i = 1; i <= n_threads; ++i) {
    pthread_t thread;
    pthread_create(&thread, nullptr,
                   [](void *arg) -> void * {
                     int *ex_tid = (int *)arg;
                     int tid = get_tid();
                     CHECK_EQ(tid, *ex_tid);
                     return nullptr;
                   },
                   &i);
    pthread_join(thread, nullptr);
  }
}

TEST_CASE("run_threads with empty list") {
  struct config config {
    false, 1000, 5, std::list<int>{},
  };
  int res = run_threads(config);
  CHECK_EQ(res, 0);
}

TEST_CASE("run_threads result nonzero") {
  struct config config {
    false, 0, 5, std::list<int>{5, 10, 15, 20, 25, 30},
  };
  int res = run_threads(config);
  CHECK_EQ(res,
           std::accumulate(config.numbers.begin(), config.numbers.end(), 0));
}

TEST_CASE("run_threads execution time lower bound") {
  struct config config {
    false, 1000, 5, std::list<int>{5, 10, 15, 20, 25, 30},
  };
  auto start = std::chrono::high_resolution_clock::now();
  run_threads(config);
  int elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - start)
                    .count();
  CHECK_LE(elapsed, config.ms_max_sleep * config.numbers.size());
}
