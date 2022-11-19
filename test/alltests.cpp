#include "gtest/gtest.h"

extern "C" {
}

int test(int argc, char **argv) {
  return 0;
}

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
