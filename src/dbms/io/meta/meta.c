#include "meta.h"

#include <assert.h>
#include <malloc.h>

static size_t meta_size_col(const size_t da_len) {
  return offsetof(struct meta, da_arr) + sizeof(struct slot_page_entry) * da_len;
}
size_t meta_size(const struct meta *meta) { return meta_size_col(meta->da_len); }

struct meta *meta_construct(const size_t da_len) {
  assert(da_len > 0);
  return calloc(sizeof(struct meta) + sizeof(struct slot_page_entry) * da_len, 1);
}

struct meta *meta_construct_init(const size_t da_len, const size_t da_slots[]) {
  struct meta *meta = meta_construct(da_len);

  meta->da_len = da_len;
  for (size_t i = 0; i < da_len; ++i) {
    assert(da_slots[i] > 0);
    meta->da_arr->slot_size = da_slots[i];
  }
  return meta;
}

void meta_destruct(struct meta **meta_ptr) {
  free(*meta_ptr);
  *meta_ptr = NULL;
}
