#pragma once

#include <inttypes.h>

#include "../util/define.h"

// Size of page or offset in page
my_defstruct(pageoff_t);
struct pageoff_t {
  uint32_t bytes;
};

// Size of body or offset in body
my_defstruct(bodyoff_t);
struct bodyoff_t {
  uint32_t bytes;
};

// Offset in file
my_defstruct(fileoff_t);
struct fileoff_t {
  int64_t bytes;
};

inline struct pageoff_t get_pageoff_t(uint32_t size) {
  return (pageoff_t){.bytes = size};
}
inline struct bodyoff_t get_bodyoff_t(uint32_t size) {
  return (bodyoff_t){.bytes = size};
}
inline struct fileoff_t get_fileoff_t(int64_t size) {
  return (fileoff_t){.bytes = size};
}
