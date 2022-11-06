#include "database.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../util.h"
#include "data.h"

// inits string size
static void typle_ram_header_init(dpt_header_ram *ramh, const dpt_header *header) {
  ramh->changed = false;
  ramh->cols = header->cols;
  ramh->fileoff = header->fileoff;
  ramh->is_inline = header->sso.is_inline;
  ramh->so = header->sso.po_ptr;
  size_to_sso(strlen(ramh->name), ramh->ssize);
}

static void typle_ram_column_init(dpt_column_ram *ramc, dpt_column_ram *next,
                                  const dpt_column *column) {
  ramc->next = next;
  ramc->type = column->type;
  ramc->limits = column->limits;
  ramc->changed = false;
  ramc->is_inline = column->sso.is_inline;
  ramc->so = column->sso.po_ptr;
  size_to_sso(strlen(ramc->name), ramc->ssize);
}

static void typle_ram_header_load(dpt_header_ram *ramh, database *db,
                                  const dp_typle *typle) {
  // Header: init fields and copy string
  if (typle->header.sso.is_inline) {
    // also puts null terminator
    ramh->name = strndup(typle->header.sso.name, SSO_MXLEN);
  } else {
    page_sso sso = typle->header.sso;
    data_page *da = da_load(db, sso.po_ptr.fileoff);
    assert(da != NULL);
    ramh->name = strndup((void *)da + sso.po_ptr.offset, sso_to_size(sso.ssize));
    da_unload(&da);
  }
  typle_ram_header_init(ramh, &typle->header);
}

static void typle_ram_header_unload(dpt_header_ram *const ramh) { free(ramh->name); }

static dpt_column_ram *typle_ram_columns_load(database *db, const dp_typle *typle) {
  dpt_column column;
  dpt_column_ram *ramc = NULL, *ram_next = NULL;
  for (size_t i = 0; i < typle->header.cols; ++i) {
    size_t irev = typle->header.cols - 1 - i;
    column = typle->columns[irev];
    ramc = my_malloc(dpt_column_ram);
    if (column.sso.is_inline) {
      ramc->name = strndup(column.sso.name, SSO_MXLEN);
    } else {
      page_sso sso = column.sso;
      data_page *da = da_load(db, sso.po_ptr.fileoff);
      assert(da != NULL);
      ramc->name = strndup((void *)da + sso.po_ptr.offset, sso_to_size(sso.ssize));
      da_unload(&da);
    }
    typle_ram_column_init(ramc, ram_next, &column);
    ram_next = ramc;
  }
  return ram_next;// as we started from end, we need to return last occurence
}

static void typle_ram_columns_unload(dpt_column_ram **const col_ptr) {
  dpt_column_ram *col = *col_ptr;
  dpt_column_ram *prev;
  while (col != NULL) {
    prev = col;
    col = col->next;
    free(prev->name);
    free(prev);
  }
  *col_ptr = NULL;
}

static inline size_t typle_size_from_typle_ram(const dp_typle_ram *ram) {
  return sizeof(dpt_header) + sizeof(dpt_column) * ram->header.cols;
}

// no-check for empty space (+ update databaseSection)
static database_index typle_index_create_sync(database *db, database_page *dp,
                                              const fileoff_t fileoff,
                                              const size_t typle_size) {
  database_index index = (database_index){.is_cleared = false,
                                          .start = dp->header.typle_start - typle_size,
                                          .end = dp->header.typle_start};
  // calculate offset where to write
  const pageoff_t off_index_last = offsetof(database_header, index_last);
  const pageoff_t off_typle_start = offsetof(database_header, typle_start);
  // update ram
  const bodyoff_t index_prev = dp->header.index_last;
  dp->header.index_last += sizeof(database_index);
  dp->header.typle_start -= typle_size;
  // alter sync header
  page_alter_pageoff_sync(db, fileoff, off_index_last, (base_page *)dp,
                          &dp->header.index_last, sizeof(dp->header.index_last));
  page_alter_pageoff_sync(db, fileoff, off_typle_start, (base_page *)dp,
                          &dp->header.typle_start, sizeof(dp->header.typle_start));
  // alter sync index
  dp_alter_bodyoff_sync(db, dp, &index, fileoff, index_prev, sizeof(index));
  return index;
}

// write inlined string and also noninlined (no sense to divide this function)
// also update typleRAM and databaseSection
static void typle_alter_sync(database *db, const fileoff_t fileoff, database_page *dp,
                             const database_index *index, dp_typle_ram *ram) {
  // for each non-inlined cell check, find place, update dp_typleRAM
  if (!ram->header.is_inline && ram->header.changed) {
    ram->header.so =
        da_alter_append(db, ram->header.name, sso_to_size(ram->header.ssize));
    ram->header.changed = false;
  }
  dpt_column_ram *ramc = ram->columns;
  while (ramc != NULL) {
    if (!ramc->is_inline && ramc->changed) {
      ramc->so = da_alter_append(db, ram->header.name, sso_to_size(ram->header.ssize));
      ramc->changed = false;
    }
    ramc = ramc->next;
  }
  // write new updated typle to file
  dp_typle *typle = dp_typle_ram_to_typle(ram);
  size_t typle_size = typle_size_from_typle_ram(ram);
  dp_alter_bodyoff_sync(db, dp, typle, fileoff, index->start, typle_size);
}

dp_typle_ram *dp_typle_ram_load(database *db, database_page *section,
                                const database_index *index) {
  dp_typle *typle = (dp_typle *)(section->body + index->start);
  dp_typle_ram *ram = my_malloc(dp_typle_ram);
  typle_ram_header_load(&ram->header, db, typle);
  ram->columns = typle_ram_columns_load(db, typle);
  return ram;
}

void dp_typle_ram_unload(dp_typle_ram **const ram_ptr) {
  dp_typle_ram *ram = *ram_ptr;
  typle_ram_header_unload(&ram->header);
  if (ram->columns != NULL) {
    typle_ram_columns_unload(&ram->columns);
  }
  free(ram);
  *ram_ptr = NULL;
}

// database_table is allocated with name and columns
// Return NULL is exists
dpt_column_ram dp_typle_column_ram_create(const int8_t type,
                                          const dpt_column_limits limits, char *name,
                                          const size_t size) {
  dpt_column_ram column = (dpt_column_ram){
      .next = NULL,
      .limits = limits,
      .type = type,
      .name = name,
      .changed = true,
  };
  size_to_sso(size, column.ssize);
  // set is_inlined flag for sso
  if (size > SSO_MXLEN) {
    column.is_inline = false;
  }
  return column;
}

dp_typle_ram dp_typle_ram_create(char *name, size_t nsize, dpt_column_ram *column,
                                 size_t cols) {
  dp_typle_ram ram = (dp_typle_ram){
      .header =
          (dpt_header_ram){
              .name = name, .fileoff = FILEOFF_NULL, .changed = true, .cols = cols},
      .columns = column,
  };
  size_to_sso(nsize, ram.header.ssize);
  // set is_inlined flag for sso
  if (nsize > SSO_MXLEN) {
    ram.header.is_inline = false;
  }
  return ram;
}

// inline with small string optimizations (doesn't do anything with strings in
// separate sections)
dp_typle *dp_typle_ram_to_typle(const dp_typle_ram *ram) {
  dp_typle *typle = calloc(1, typle_size_from_typle_ram(ram));
  typle->header.cols = ram->header.cols;
  // Copy header if is not inlined
  if (sso_to_size(ram->header.ssize) > SSO_MXLEN) {
    typle->header.sso.is_inline = false;
    memcpy(typle->header.sso.ssize, ram->header.ssize, SSO_SSIZE_SIZE);
  } else {
    typle->header.sso.is_inline = true;
    memcpy(typle->header.sso.name, ram->header.name, sso_to_size(ram->header.ssize));
  }
  // Set columns
  dpt_column *colstart = (void *)typle + offsetof(dp_typle, columns);
  dpt_column_ram *ramcurrent = ram->columns;
  for (int i = 0; i < ram->header.cols; ++i, ramcurrent = ramcurrent->next) {
    dpt_column *column = colstart + i;
    column->limits = ramcurrent->limits;
    column->type = ramcurrent->type;
    if (sso_to_size(ramcurrent->ssize) > SSO_MXLEN) {
      column->sso.is_inline = false;
      memcpy(column->sso.ssize, ramcurrent->ssize, SSO_SSIZE_SIZE);
    } else {
      column->sso.is_inline = true;
      memcpy(column->sso.name, ramcurrent->name, sso_to_size(ramcurrent->ssize));
    }
  }
  return typle;
}

dp_typle_ram_wr dp_table_select(database *db, const char *name) {
  assert(db->is_opened);
  database_page *dp = dp_load(db, db->dst.dp.first);
  dp_typle_ram_wr result = {.typle = NULL};
  fileoff_t fileoff = db->dst.dp.first;
  while (dp != NULL) {
    bodyoff_t start = 0;
    const bodyoff_t end = dp->header.index_last;
    dpt_header_ram header;
    while (start < end) {
      database_index *index = (database_index *)(dp->body + start);
      typle_ram_header_load(&header, db, (dp_typle *)(dp->body + index->start));
      if (strcmp(header.name, name) == 0) {
        result = (dp_typle_ram_wr){.ifileoff = fileoff,
                                   .ibodyoff = start,
                                   .typle = dp_typle_ram_load(db, dp, index)};
        break;
      }
      typle_ram_header_unload(&header);
      start += sizeof(database_index);
    }
    // if result found -> close resources and return result
    if (result.typle != NULL) {
      dp_unload(&dp);
      return result;
    } else {
      database_page_wr wrapper = dp_load_next(db, dp);
      fileoff = wrapper.fileoff;
      dp_unload(&dp);
      dp = wrapper.dp;
    }
  }
  return (dp_typle_ram_wr){.ifileoff = FILEOFF_NULL, .ibodyoff = 0, .typle = NULL};
}

// if found -> return NULL as dp_typleRAMWr.typle (changed for each is set to
// true)
dp_typle_ram_wr dp_table_create(database *database, dp_typle_ram *ram) {
  dp_typle_ram_wr result = dp_table_select(database, ram->header.name);
  if (result.typle != NULL) {
    dp_typle_ram_unload(&result.typle);
    return result;
  }
  size_t typle_size = typle_size_from_typle_ram(ram);
  // Put table in current section or create new
  database_page *dp = dp_load(database, database->dst.dp.last);
  result.ifileoff = database->dst.dp.last;
  size_t space_left = dp_get_space_left(dp);
  if (space_left <= typle_size + sizeof(database_index)) {
    dp_unload(&dp);
    database_page_wr wrapper = dp_create(database, dp, database->dst.dp.last);
    dp = wrapper.dp;
    assert(dp_get_space_left(dp) <= typle_size + sizeof(database_index));
    result.ifileoff = wrapper.fileoff;
  }
  // Append current section with typle + add notinlined fields
  database_index index =
      typle_index_create_sync(database, dp, result.ifileoff, typle_size);
  typle_alter_sync(database, result.ifileoff, dp, &index, ram);
  result.ibodyoff = dp->header.index_last - sizeof(database_index);
  result.typle = ram;
  return result;
}

dp_typle_ram_wr dp_table_drop(database *db, const char *name) {
  dp_typle_ram_wr wrapper = dp_table_select(db, name);
  if (wrapper.typle == NULL) {
    return wrapper;
  }
  database_index index = {.is_cleared = true};
  dp_alter_bodyoff(db, &index, wrapper.ifileoff, wrapper.ibodyoff, sizeof(index));
  return wrapper;
}
