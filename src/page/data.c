#include "data.h"

#include "internals.h"

struct data_page *da_construct(struct pageoff_t size) {
  return (data_page *)page_construct(size, PAGE_DATA);
}

void da_destruct(struct data_page **page_ptr) { page_destruct((base_page **)page_ptr); }
