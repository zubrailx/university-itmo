#include "database.h"

size_t DATABASE_PAGE_SIZE = 1024;

// RAM
struct database_page *dp_construct(struct page_size size) {
  return (database_page *)page_construct(size, PAGE_DATABASE);
}

void dp_destruct(struct database_page **page_ptr) {
  page_destruct((base_page **)page_ptr);
}

// Operations

// FILE
PAGE_DEFAULT_CREATE_IMPL(struct database_page, dp)
PAGE_DEFAULT_ALTER_IMPL(struct database_page, dp)
PAGE_DEFAULT_DROP_IMPL(struct database_page, dp)
