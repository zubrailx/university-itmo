#pragma once

#include <dbms/internals/page_loc.h>
#include <stddef.h>

struct slot_page_entry {
  fileoff_t last;
  uint32_t slot_size;
  uint32_t slot_count;
};

/* Stored in file */
typedef struct meta {
  int32_t open_count;
  // file position (like heap position in malloc)
  fileoff_t pos_empty;

  // Database Page
  fileoff_t dp_last;

  // Last page of page allocator
  fileoff_t free_last;

  // Last pages of data distributor
  size_t slot_len;
  struct slot_page_entry slot_entries[];
} meta;

size_t meta_size(const struct meta *meta);

struct meta *meta_construct(size_t slot_len);

struct meta *meta_construct_init(size_t slot_len, const size_t slot_size[],
                                 const size_t slot_counts[]);

void meta_destruct(struct meta **meta_ptr);
