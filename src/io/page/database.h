#pragma once

#include "base.h"

/*
Structure: database_page
  |------------------|
  |HHHHHHH|T TTT  TTT| H - header
  |------------------| T - typles
  |                  | I - indexes to get typles because not fixed size
  |------------------|
  |              IIII|
  |------------------|

*/
my_defstruct(database_header);
struct database_header {
  struct base_header base;
  fileoff_t next;
  fileoff_t previous;
  bodyoff_t index_start;// farthest to end of page index
  bodyoff_t typle_end;  // after the last element
};

my_defstruct(database_page);
struct database_page {
  struct database_header header;
  uint8_t body[];
} __attribute__((packed));

// Bodyoff to pageoff
BODYOFF_TO_PAGEOFF(database_header, dp)
PAGEOFF_TO_BODYOFF(database_header, dp)

struct database_page *dp_construct(struct pageoff_t size);
void dp_destruct(struct database_page **page_ptr);
