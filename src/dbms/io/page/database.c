#include "database.h"

struct database_page *dp_construct(struct pageoff_t size) {
  return (database_page *)page_construct(size, PAGE_DATABASE);
}

void dp_destruct(struct database_page **page_ptr) {
  page_destruct((base_page **)page_ptr);
}
