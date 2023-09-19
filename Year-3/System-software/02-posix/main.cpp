#include <iostream>
#include "producer_consumer.h"

int main([[maybe_unused]] int argc, [[maybe_unused]] const char **argv) {
  // not to wait until user input
  if (argc < 3) {
    throw std::invalid_argument("Argument must be three at least three");
  }

  std::string snums;
  std::getline(std::cin, snums);
  struct config config = config::build(argc, argv, std::move(snums));

  std::cout << run_threads(config) << std::endl;

  return 0;
}
