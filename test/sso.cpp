#include <gtest/gtest.h>

extern "C" {
#include "../src/sso.h"
}

TEST(SSO, Test) {
  size_t size1 = 212140551414;
  char c[SSO_SSIZE_SIZE];
  size_to_sso(size1, c);
  size_t size2 = sso_to_size(c);
  EXPECT_EQ(size1, size2);
}
