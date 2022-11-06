#pragma once

#include "../po_ptr.h"
#include "base.h"

extern pageoff_t DATA_SECTION_SIZE;

// Section for storing Data (like string)
my_defstruct(data_header);
struct data_header {
  struct base_page base;
  bodyoff_t stored_last;
};

my_defstruct(data_page);
struct data_page {
  struct data_header header;
  char *body;
};

my_defstruct(data_page_wr);
struct data_page_wr {
  fileoff_t fileoff;
  struct data_page *da;
};

data_page_wr da_create(database *database);
void da_alter(database *database, fileoff_t fileoff, data_page *da);
void da_alter_bodyoff(database *database, fileoff_t fileoff, bodyoff_t bodyoff,
                      const void *data, size_t size);
po_ptr da_alter_append(database *database, const void *data, size_t size);
void da_drop(database *database, fileoff_t fileoff);

data_page *da_load(database *database, fileoff_t fileoff);
void da_unload(data_page **ds);
