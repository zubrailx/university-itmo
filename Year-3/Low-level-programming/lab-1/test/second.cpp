#include <gtest/gtest.h>

#include "class.h"

TEST(Class, Imported2) {
  A a;
  a.a = 10;
  EXPECT_EQ(a.a, 10);
}
