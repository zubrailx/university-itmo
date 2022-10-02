#pragma once

#include <inttypes.h>

#define SECTION_OFFSET_NULL 0


typedef struct BaseSection {
  uint8_t type;
  uint32_t size;
} BaseSection;

typedef uint64_t fileoff_t;
typedef uint32_t sectoff_t;

enum SectionTypes {
  TYPE_UNKNOWN,
  TYPE_DATABASE,
  TYPE_HASHTABLE,
  TYPE_BACKET,
  TYPE_PAGE,
  TYPE_PAGEDATA,
  TYPE_TABLE,
  TYPE_TMP,
  TYPE_DUMPED,
};
