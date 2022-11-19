#pragma once

#include <inttypes.h>
#include <stddef.h>

typedef int64_t fileoff_t;
typedef uint32_t pageoff_t;
typedef uint32_t bodyoff_t;

#define FILEOFF_NULL ((fileoff_t)0)
// Because 0 points to current page
#define PAGE_START_INDEX 1
#define PAGE_CURRENT ((fileoff_t)0)
