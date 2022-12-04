#include "meta.h"

#include <assert.h>
#include <malloc.h>

static size_t meta_size_col(const size_t slot_len) {
  return offsetof(struct meta, slot_entries) + sizeof(struct slot_page_entry) * slot_len;
}
size_t meta_size(const struct meta *meta) { return meta_size_col(meta->slot_len); }

struct meta *meta_construct(const size_t slot_len) {
  assert(slot_len > 0);
  return calloc(sizeof(struct meta) + sizeof(struct slot_page_entry) * slot_len, 1);
}

struct meta *meta_construct_init(const size_t slot_len, const size_t slot_sizes[],
                                 const size_t slot_counts[]) {
  struct meta *meta = meta_construct(slot_len);
  // init
  meta->pos_empty.bytes = meta_size_col(slot_len);
  meta->slot_len = slot_len;
  for (size_t i = 0; i < slot_len; ++i) {
    assert(slot_sizes[i] > 0);
    meta->slot_entries->slot_size = slot_sizes[i];
    meta->slot_entries->slot_count = slot_counts[i];
  }
  return meta;
}

void meta_destruct(struct meta **meta_ptr) {
  free(*meta_ptr);
  *meta_ptr = NULL;
}
