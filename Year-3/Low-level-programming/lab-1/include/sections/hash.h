#pragma once

#include "base.h"

// HASH
// HashList
typedef struct HashList HashList;

struct HashList {
  /* offset in file */
  fileoff_t next_list;
  fileoff_t fileoff_element;
};

// Backet
typedef struct HashBacket {
  fileoff_t hash_list_first;
} HashBacket;

// BacketSectionHeader
typedef struct BacketHeader {
  BaseSection base_section;
  uint32_t backet_end_noninc;
} BacketHeader;

typedef struct BacketSection {
  BacketHeader header;
  void *body;
} BacketSection;

// HASHTABLE
typedef struct HashTableHeader {
  BaseSection base_section;

  uint32_t bucker_count;
  uint64_t element_count;
  uint64_t max_elements;
} HashTableHeader;

typedef struct HashTableSection {
  HashTableHeader header;
  void *body;
} HashTableSection;
