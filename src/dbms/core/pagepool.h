#pragma once

// TODO: hash-map or btree to get from fileoff_t address in memory and to manage pages
// in RAM will be effective because strings are stored in separate sections in case of
// their big size

#include "../io/page/p_base.h"

struct pagepool {
  // without duplicates
  struct base_page *pages;
};

struct base_page *pagepool_find(fileoff_t fileoff);
void pagepool_insert(fileoff_t fileoff, base_page *page);
