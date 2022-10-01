#include <gtest/gtest.h>

#include "class.h"

TEST(Class, Imported) {
  A a;
  a.a = 5;
  EXPECT_EQ(a.a, 10);
}

