#pragma once

#include <stddef.h>
#include <stdalign.h>

inline size_t align_get_padding(size_t offset, size_t align) {
  return -offset & (align - 1);
}

#define _ALIGNED_PADDING(offset, align) (-offset & (align - 1))
