#include "database.h"

#include <assert.h>
#include <malloc.h>
#include <string.h>

#include "../util.h"

pageoff_t DATABASE_PAGE_SIZE = 1024;

inline size_t dp_get_space_left(const database_page *dbs) {
  return dbs->header.typle_start - dbs->header.index_last;
}

inline size_t dp_body_size(const database_page *dbs) {
  return dbs->header.base.size - sizeof(database_page);
}

inline bodyoff_t dp_get_bodyoff(pageoff_t pageoff) {
  return pageoff - sizeof(database_page);
}
inline pageoff_t dp_get_pageoff(bodyoff_t bodyoff) {
  return bodyoff + sizeof(database_page);
}

static void dp_init(database_page *dp, fileoff_t previous) {
  database_header *dh = &dp->header;
  // Init database section
  dh->base.type = PAGE_DATABASE;
  dh->base.size = DATABASE_PAGE_SIZE;
  dh->next = FILEOFF_NULL;
  dh->previous = previous;
  dh->index_last = 0;
  dh->typle_start = dp_get_bodyoff(DATABASE_PAGE_SIZE);
}

// load + store in file
database_page_wr dp_create(database *db, database_page *prev, fileoff_t prev_pos) {
  database_page *dp = (database_page *)page_malloc(DATABASE_PAGE_SIZE);
  dp_init(dp, FILEOFF_NULL);
  fileoff_t next_pos = page_create(db, (base_page *)dp);
  if (prev != FILEOFF_NULL) {
    prev->header.next = next_pos;
    page_alter_pageoff(db, prev_pos, offsetof(database_header, next),
                       &prev->header.next, sizeof(prev->header.next));
  }
  return (database_page_wr){.dp = dp, .fileoff = next_pos};
}

void dp_alter(database *database, const fileoff_t fileoff, const database_page *dp) {
  page_alter(database, fileoff, (base_page *)dp);
}

void dp_alter_bodyoff(database *database, const void *data, fileoff_t fileoff,
                      bodyoff_t offset, size_t size) {
  page_alter_pageoff(database, fileoff, dp_get_pageoff(offset), data, size);
}

void dp_alter_bodyoff_sync(database *database, database_page *ds, const void *data,
                           fileoff_t fileoff, bodyoff_t offset, size_t size) {
  page_alter_pageoff_sync(database, fileoff, dp_get_pageoff(offset), (base_page *)ds,
                          data, size);
}

void dp_drop(database *database, fileoff_t fileoff) {
  database_header *curr =
      (database_header *)page_header_load(database, fileoff, sizeof(database_header));
  page_drop_sync(database, fileoff, (base_page *)curr);
  // update relatives
  if (curr->next != FILEOFF_NULL) {
    page_alter_pageoff(database, curr->next, offsetof(database_header, previous),
                       &curr->previous, sizeof(curr->previous));
  }
  if (curr->previous != FILEOFF_NULL) {
    page_alter_pageoff(database, curr->previous, offsetof(database_header, next),
                       &curr->next, sizeof(curr->next));
  }
}

// NULLABLE
database_page_wr dp_load_next(database *database, const database_page *current) {
  fileoff_t pos = current->header.next;
  if (pos == FILEOFF_NULL) {
    return (database_page_wr){.fileoff = FILEOFF_NULL, NULL};
  }
  return (database_page_wr){.fileoff = pos,
                            .dp = (database_page *)page_load(database, pos)};
}

database_page *dp_load(database *database, fileoff_t fileoff) {
  return (database_page *)page_load_check_type(database, fileoff, PAGE_DATABASE);
}

void dp_unload(database_page **dp) { page_unload((base_page **)dp); }
