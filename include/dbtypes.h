#pragma once

#include <inttypes.h>

typedef uint64_t fileoff_t;
typedef uint32_t sectoff_t;
typedef uint32_t bodyoff_t;

#define SECTION_OFFSET_NULL 0
// Because 0 points to current page
#define SECTION_START_INDEX 1
#define SECTION_CURRENT_PTR 0
