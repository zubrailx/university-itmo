#pragma once

#include <list>
#include <string>

struct config {
  bool is_debug = false;
  int ms_max_sleep = 1000;
  int n_consumer = 3;
  std::list<int> numbers;

 public:
  static struct config build(int argc, const char **argv,
                             const std::string &numbers);
};

// the declaration of run threads can be changed as you like
int get_tid();
int run_threads(const struct config &config);
