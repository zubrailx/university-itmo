#include "base_fm.h"

#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "../dbms/dbfile.h"
#include "base.h"

static base_header DUMPED = (base_header){.type = PAGE_DUMPED};

void page_load_size(page_size *size, dbfile *db, fileoff_t page_start) {
  FILE *file = dbfile_file(db);
  fseek(file, page_start + offsetof(base_header, size), SEEK_SET);
  assert(fread(size, sizeof(page_size), 1, file));
}

void page_load(base_page *base, dbfile *db, fileoff_t page_start) {
  FILE *file = dbfile_file(db);
  fseek(file, page_start, SEEK_SET);
  assert(fread(base, base->header.size.bytes, 1, file));
}

void page_create(struct base_page *base, struct dbfile *db, fileoff_t page_start) {
  page_alter(base, db, page_start);
}

void page_alter(struct base_page *base, struct dbfile *db, fileoff_t page_start) {
  FILE *file = dbfile_file(db);
  fseek(file, page_start, SEEK_SET);
  fwrite(base, base->header.size.bytes, 1, file);
}

void page_drop(struct dbfile *db, fileoff_t page_start) {
  FILE *file = dbfile_file(db);
  fseek(file, page_start + offsetof(base_header, type), SEEK_SET);
  fwrite(&DUMPED.type, sizeof(DUMPED.type), 1, file);
}
