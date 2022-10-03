#pragma once

#include "database_meta.h"

typedef struct BaseSection BaseSection;
typedef struct SectionOffsetPointer SOPointer;

struct BaseSection {
  uint8_t type;
  uint32_t size;
  uint32_t number;
};

struct SectionOffsetPointer {
  uint32_t number;
  uint32_t offset;
};

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

void *section_load(Database *database, const fileoff_t offset);
