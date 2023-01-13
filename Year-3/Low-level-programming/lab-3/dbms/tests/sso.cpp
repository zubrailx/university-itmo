#include <gtest/gtest.h>

extern "C" {
#include <dbms/internals/sso.h>
}

TEST(sso, test) {
  size_t size1 = 212140551414;
  unsigned char c[SSO_SSIZE_SIZE];
  size_to_sso(size1, c);
  size_t size2 = sso_to_size(c);
  EXPECT_EQ(size1, size2);
}
