#pragma once

#include <inttypes.h>
#include <stdbool.h>

// inline int64_t op_foff_sum(int64_t a, int64_t b) { return a + b; }

// inline int64_t op_foff_sub(int64_t a, int64_t b) { return a - b; }

// Size of page or offset in page
typedef struct pageoff_t {
  uint32_t bytes;
} pageoff_t;

// Size of body or offset in body
typedef struct bodyoff_t {
  uint32_t bytes;
} bodyoff_t;

// Offset in file
typedef struct fileoff_t {
  int64_t bytes;
} fileoff_t;

#define FILEOFF_NULL ((struct fileoff_t){.bytes = 0})
#define SIZE_DEFAULT ((struct pageoff_t){.bytes = 0})

inline bool fileoff_is_null(struct fileoff_t off) { return off.bytes == 0; }

inline struct pageoff_t get_pageoff_t(uint32_t size) {
  return (pageoff_t){.bytes = size};
}
inline struct bodyoff_t get_bodyoff_t(uint32_t size) {
  return (bodyoff_t){.bytes = size};
}
inline struct fileoff_t get_fileoff_t(int64_t size) {
  return (fileoff_t){.bytes = size};
}

// inline fileoff_t op_fileoff_t(int64_t (*op)(int64_t, int64_t), fileoff_t a,
//                               fileoff_t b) {
//   return get_fileoff_t(op(a.bytes, b.bytes));
// }
