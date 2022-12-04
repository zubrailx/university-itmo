#include "p_slot.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <util/internals.h>

typedef uint32_t slot_number;

EXTERN_INLINE_BODYOFF_TO_PAGEOFF(struct slot_page, body, sp)
EXTERN_INLINE_PAGEOFF_TO_BODYOFF(struct slot_page, body, sp)

pageoff_t sp_size(const size_t slot_size, const size_t slot_count) {
  size_t size = slot_count * (slot_size + sizeof(slot_number));
  return sp_body_page(get_bodyoff_t(size));
}

// constructors/destructors
PAGE_CONSTRUCT_DEFAULT_IMPL(struct slot_page, sp, PAGE_SLOT)
PAGE_DESTRUCT_DEFAULT_IMPL(struct slot_page, sp)

slot_page *sp_construct_slot(const size_t slot_size, const size_t slot_count) {
  return sp_construct(sp_size(slot_size, slot_count));
}

struct slot_page *sp_construct_slot_init(const size_t slot_size,
                                         const size_t slot_count) {
  slot_page *page = sp_construct_slot(slot_size, slot_count);
  // init
  page->header.slot_size = slot_size;
  page->header.slot_count = slot_count;
  page->header.slot_start = page->header.base.size;
  return page;
}

bool sp_full(const slot_page *page) {
  return page->header.slot_start.bytes == page->header.base.size.bytes;
}

bool sp_empty(const slot_page *page) {
  slot_header header = page->header;
  return header.slot_start.bytes ==
         header.base.size.bytes - sizeof(slot_number) * header.slot_count;
}

static slot_number sp_pop(slot_page *page) {
  slot_number num = *(slot_number *)((void *)page + page->header.slot_start.bytes);
  page->header.slot_start.bytes += sizeof(slot_number);
  return num;
}

static void sp_push(slot_page *page, const slot_number num) {
  page->header.slot_start.bytes -= sizeof(slot_number);
  *(slot_number *)((void *)page + page->header.slot_start.bytes) = num;
}

static slot_number sp_slot_num(const slot_page *page, const pageoff_t pageoff) {
  return sp_page_body(pageoff).bytes / page->header.slot_size;
}

static pageoff_t sp_slot_start(const slot_page *page, const slot_number num) {
  size_t slot_size = page->header.slot_size;
  return get_pageoff_t(offsetof(slot_page, body) + slot_size * num);
}

// If returns 0 then no available space left
pageoff_t sp_insert_data(struct slot_page *page, const void *data, size_t size) {
  assert(size <= page->header.slot_size);

  if (sp_full(page)) {
    return get_pageoff_t(0);
  } else {
    slot_number num = sp_pop(page);
    pageoff_t ins_off = sp_slot_start(page, num);
    void *ins_pos = (void *)page + ins_off.bytes;
    memcpy(ins_pos, data, size);
    return ins_off;
  }
}

void sp_remove_data(struct slot_page *page, const pageoff_t pageoff) {
  assert(!sp_empty(page));
  // just add spta slot (without zeroing memory)
  slot_number num = sp_slot_num(page, pageoff);
  sp_push(page, num);
}
