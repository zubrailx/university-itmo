#pragma once

#include <inttypes.h>
#include <stdbool.h>

#include "../database.h"
// NOTE: First - section header offset == 0, this means that freeing section
// header is NOTE: the same as freeing section

my_defstruct(base_page);
struct base_page {
  uint8_t type;
  uint32_t size;
};

enum page_types {
  TYPE_UNKNOWN,
  TYPE_DATABASE,
  TYPE_HASHTABLE,
  TYPE_BACKET,
  TYPE_PAGE,
  TYPE_DATA,
  TYPE_TABLE,
  TYPE_TMP,
  TYPE_DUMPED,
};

// RAM
base_page *page_malloc(const pageoff_t sect_size);
base_page *page_load(const database *database, const fileoff_t offset);
base_page *page_load_check_type(const database *database, const fileoff_t fileoff,
                                int8_t type);
void page_unload(base_page **section);

base_page *page_header_load(const database *database, const fileoff_t fileoff,
                            size_t size);

// FILE or FILE + RAM(sync)
fileoff_t page_create(database *database, const base_page *section);

bool page_alter(database *database, const fileoff_t fileoff, const base_page *sect);
bool page_alter_pageoff(database *database, const fileoff_t fileoff,
                        const pageoff_t offset, const void *data, const size_t size);
bool page_alter_pageoff_sync(database *database, const fileoff_t fileoff,
                             const pageoff_t offset, base_page *base, const void *data,
                             const size_t size);

bool page_drop(database *database, const fileoff_t fileoff);
bool page_drop_sync(database *database, const fileoff_t fileoff, base_page *section);
