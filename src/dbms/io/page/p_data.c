#include "p_data.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <util/internals.h>

typedef uint32_t slot_number;

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(struct data_page, body, da)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(struct data_page, body, da)

static pageoff_t da_size(const size_t slot_size, const size_t slot_count) {
  size_t size = slot_count * (slot_size + sizeof(slot_number));
  return da_body_page(get_bodyoff_t(size));
}

data_page *da_construct_slot(const size_t slot_size, const size_t slot_count) {
  return (data_page *)page_construct(da_size(slot_size, slot_count), PAGE_DATA);
}
PAGE_DESTRUCT_DEFAULT_IMPL(data_page, da)

struct data_page *da_construct_slot_init(const size_t slot_size,
                                         const size_t slot_count) {
  data_page *page = da_construct(slot_size, slot_count);
  // init
  page->header.slot_size = slot_size;
  page->header.slot_count = slot_count;
  page->header.slot_start = page->header.base.size;
  return page;
}

bool da_is_full(const data_page *page) {
  return page->header.slot_start.bytes == page->header.base.size.bytes;
}

bool da_is_empty(const data_page *page) {
  data_header header = page->header;
  return header.slot_start.bytes ==
         header.base.size.bytes - sizeof(slot_number) * header.slot_count;
}

static slot_number da_pop(data_page *page) {
  slot_number num = *(slot_number *)((void *)page + page->header.slot_start.bytes);
  page->header.slot_start.bytes += sizeof(slot_number);
  return num;
}

static void da_push(data_page *page, const slot_number num) {
  page->header.slot_start.bytes -= sizeof(slot_number);
  *(slot_number *)((void *)page + page->header.slot_start.bytes) = num;
}

static slot_number da_slot_num(const data_page *page, const pageoff_t pageoff) {
  return da_page_body(pageoff).bytes / page->header.slot_size;
}

static pageoff_t da_slot_start(const data_page *page, const slot_number num) {
  size_t slot_size = page->header.slot_size;
  return get_pageoff_t(offsetof(data_page, body) + slot_size * num);
}

// If returns 0 then no available space left
pageoff_t da_insert_data(struct data_page *page, const void *data, size_t size) {
  assert(size <= page->header.slot_size);

  if (da_is_full(page)) {
    return get_pageoff_t(0);
  } else {
    slot_number num = da_pop(page);
    pageoff_t ins_off = da_slot_start(page, num);
    void *ins_pos = (void *)page + ins_off.bytes;
    memcpy(ins_pos, data, size);
    return ins_off;
  }
}

void da_remove_data(struct data_page *page, const pageoff_t pageoff) {
  assert(!da_is_empty(page));
  // just add data slot (without zeroing memory)
  slot_number num = da_slot_num(page, pageoff);
  da_push(page, num);
}
