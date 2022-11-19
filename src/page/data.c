#include "data.h"

size_t DATA_PAGE_SIZE = 1024;

// RAM
struct data_page *da_construct(struct page_size size) {
  return (data_page *)page_construct(size, PAGE_DATA);
}

void da_destruct(struct data_page **page_ptr) { page_destruct((base_page **)page_ptr); }

// FILE
PAGE_DEFAULT_CREATE_IMPL(struct data_page, da)
PAGE_DEFAULT_ALTER_IMPL(struct data_page, da)
PAGE_DEFAULT_DROP_IMPL(struct data_page, da)
