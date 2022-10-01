#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "util.h"

typedef struct BaseSection {
  uint8_t type;
  uint32_t size;
} BaseSection;

// DATABASE
typedef struct DatabaseHeader {
  BaseSection base_section;
  fileoff_t next;
} DatabaseHeader;

typedef struct DatabaseSection {
  DatabaseHeader header;
  void *body;
} DatabaseSection;

// TMP
typedef struct TmpHeader {
  BaseSection base_section;
  fileoff_t next;
} TmpHeader;

typedef struct TmpSection {
  TmpHeader tmp_header;
  void *body;
} TmpSection;

// PAGE
typedef struct PageSection PageSection;

// PageHeader
typedef struct PageHeader {
  BaseSection base_section;
  fileoff_t page_previous;
  fileoff_t page_next;
  /* non-inclusive (off_typle_info_end - off_typle_info + 1) == empty_size*/
  uint32_t off_typle_info_end;
  uint32_t off_typle_end;
} PageHeader;

// PageBody
typedef struct PageFlags {
  /* Is entry is present or deleted */
  bool is_valid;
} PageFlags;

typedef struct PageLinePointer {
  /* Offset start inclusive, which points on start */
  uint32_t off_start_inc;
  /* Offset end non inclusive, which points over end */
  uint32_t off_end_noninc;
  PageFlags flags;
} PageLinePointer;

// Page
struct PageSection {
  PageHeader header;
  void *body;
};

// DATA
typedef struct DataHeader {
  BaseSection base_section;
  fileoff_t next;
} DataHeader;

typedef struct DataSection {
  DataHeader header;
  void *body;
} Data;

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

// Page
typedef struct BacketPageHeader {
  BaseSection base_section;
  uint32_t backet_end_noninc;
} BacketPageHeader;

typedef struct BacketSection {
  BacketPageHeader backet_page_header;
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

// TABLE
typedef struct TableHeader {
  uint32_t page_count;
  fileoff_t page_last_filled;
  fileoff_t page_first;
} TableHeader;

typedef struct TableSection {
  TableHeader header;
  void *body;
} TableSection;
