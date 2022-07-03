#include "Buffer.h"
#include "BufferView.h"
#include "gtest/gtest.h"

TEST(Buffer, Empty) {
  Buffer buffer;

  EXPECT_EQ(buffer.get_data(), nullptr);
  EXPECT_EQ(buffer.get_size(), 0);
  EXPECT_EQ(buffer.get_capacity(), 0);
}

TEST(Buffer, CapacityConstructor) {
  Buffer buffer(100);

  EXPECT_NE(buffer.get_data(), nullptr);
  EXPECT_EQ(buffer.get_size(), 0);
  EXPECT_EQ(buffer.get_capacity(), 100);
}

TEST(Buffer, DataSizeConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer buffer(array, array_size);

  EXPECT_NE(buffer.get_data(), nullptr);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(buffer.get_data())[i], array[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(buffer.get_size(), array_size);
  EXPECT_EQ(buffer.get_capacity(), array_size);
}

TEST(Buffer, BufferViewConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer buffer(BufferView(array, array_size));

  EXPECT_NE(buffer.get_data(), array);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(buffer.get_data())[i], array[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(buffer.get_capacity(), array_size);
  EXPECT_EQ(buffer.get_size(), array_size);
}

TEST(Buffer, CopyConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer first(array, array_size);
  Buffer second(first);

  EXPECT_NE(first.get_data(), second.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(second.get_data())[i],
              static_cast<int *>(first.get_data())[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(second.get_capacity(), first.get_capacity());
  EXPECT_EQ(first.get_capacity(), array_size);
  EXPECT_EQ(second.get_size(), first.get_size());
  EXPECT_EQ(first.get_size(), array_size);
}

TEST(Buffer, MoveConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer first(array, array_size);
  void *old_data = first.get_data();
  size_t old_size = first.get_size();
  size_t old_capacity = first.get_capacity();
  Buffer second(std::move(first));

  EXPECT_EQ(second.get_data(), old_data);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(second.get_data())[i], array[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(second.get_capacity(), old_capacity);
  EXPECT_EQ(second.get_size(), old_size);
}

TEST(Buffer, AssignOperator) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer first(array, array_size);
  Buffer second = first;

  EXPECT_NE(second.get_data(), first.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(second.get_data())[i],
              static_cast<int *>(first.get_data())[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(first.get_capacity(), second.get_capacity());
  EXPECT_EQ(first.get_capacity(), array_size);
  EXPECT_EQ(first.get_size(), second.get_size());
  EXPECT_EQ(first.get_size(), array_size);
}

TEST(Buffer, AssignMoveOperator) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer first(array, array_size);
  void *old_data = first.get_data();
  size_t old_capacity = first.get_capacity();
  size_t old_size = first.get_size();
  Buffer second = std::move(first);

  EXPECT_EQ(second.get_data(), old_data);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<int *>(second.get_data())[i], array[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(old_capacity, second.get_capacity());
  EXPECT_EQ(old_size, second.get_size());
}

TEST(Buffer, SetCapacityEquals) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);

  Buffer b(array, array_size);
  b.set_capacity(array_size);

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], static_cast<int *>(b.get_data())[i]);
  }
  EXPECT_EQ(b.get_capacity(), array_size);
  EXPECT_EQ(b.get_size(), array_size);
}

TEST(Buffer, SetCapacityHigher) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);

  Buffer b(array, array_size);
  b.set_capacity(2 * array_size);

  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], static_cast<int *>(b.get_data())[i]);
  }
  EXPECT_EQ(b.get_capacity(), 2 * array_size);
  EXPECT_EQ(b.get_size(), array_size);
}

TEST(Buffer, SetCapacityLower) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);

  Buffer b(array, array_size);
  b.set_capacity(array_size / 2);

  for (int i = 0; i < 10 / 2; ++i) {
    EXPECT_EQ(array[i], static_cast<int *>(b.get_data())[i]);
  }
  // wanted expect_death or expect_exit, but idk why it didn't work
  EXPECT_EQ(b.get_capacity(), array_size / 2);
  EXPECT_EQ(b.get_size(), array_size / 2);
}

TEST(BufferView, Empty) {
  BufferView bv;
  EXPECT_EQ(bv.get_data(), nullptr);
  EXPECT_EQ(bv.get_size(), 0);
}

TEST(BufferView, DataSizeConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  BufferView bv(array, array_size);

  EXPECT_EQ(bv.get_data(), array);
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(static_cast<const int *>(bv.get_data())[i], array[i]);
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(bv.get_size(), array_size);
}

TEST(BufferView, BufferConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  Buffer b(array, array_size);
  BufferView bv(b);

  EXPECT_EQ(bv.get_data(), b.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(bv.get_size(), b.get_size());
  EXPECT_EQ(b.get_size(), array_size);
}

TEST(BufferView, CopyConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  BufferView first(array, array_size);
  BufferView second(first);

  EXPECT_EQ(first.get_data(), second.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(first.get_size(), second.get_size());
  EXPECT_EQ(second.get_size(), array_size);
}

TEST(BufferView, MoveConstructor) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  BufferView first(array, array_size);
  const void *old_data = first.get_data();
  const size_t old_size = first.get_size();
  BufferView second(std::move(first));

  EXPECT_EQ(old_data, second.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(old_size, second.get_size());
}

TEST(BufferView, AssignOperator) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  BufferView first(array, array_size);
  BufferView second = first;

  EXPECT_EQ(first.get_data(), second.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(first.get_size(), second.get_size());
  EXPECT_EQ(array_size, second.get_size());
}

TEST(BufferView, MoveOperator) {
  int array[10]{1, 2, 3};
  size_t array_size = 10 * sizeof(int);
  int array_check[10];
  ::memcpy(array_check, array, array_size);

  BufferView first(array, array_size);
  const void *old_data = first.get_data();
  const size_t old_size = first.get_size();
  BufferView second = std::move(first);

  EXPECT_EQ(old_data, second.get_data());
  for (int i = 0; i < 10; ++i) {
    EXPECT_EQ(array[i], array_check[i]);
  }
  EXPECT_EQ(old_size, second.get_size());
}
