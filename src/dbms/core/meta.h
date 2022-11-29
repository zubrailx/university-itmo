#pragma once

#include <stdbool.h>

#include <util/internals.h>

/* Stored in file */
typedef struct dbmeta {
  struct {// database_page
    fileoff_t first;
    fileoff_t last;
  } dp;
  struct {
    fileoff_t first;
    fileoff_t last;
  } da;// data_page
  struct {
    fileoff_t first;
    fileoff_t last;
  } dumped;// empty pages (page_container)
  fileoff_t pos_empty;
} dbmeta;

struct dbmeta *meta_construct();
void meta_destruct(struct dbmeta **meta_ptr);
void meta_init_pages(struct dbmeta *meta, fileoff_t dp_first, fileoff_t da_first);

fileoff_t meta_get_next_pos(const dbmeta *meta);
void meta_set_dp_last_pos(dbmeta *meta, const fileoff_t dp_last);
void meta_set_da_last_pos(dbmeta *meta, const fileoff_t da_last);
