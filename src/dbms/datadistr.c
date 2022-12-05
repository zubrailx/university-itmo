#include "datadistr.h"
#include "core/dbfile.h"
#include "core/dbmeta.h"
#include "core/dbms.h"
#include "io/p_container.h"
#include "io/p_slot.h"
#include "page.h"
#include "pagealloc.h"
#include "string.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void dbms_dd_create_close(struct dbms *dbms) {
  meta *meta = dbms->meta;
  for (size_t i = 0; i < meta->slot_len; ++i) {
    meta->slot_entries[i].last =
        dbms_container_create_close(dbms, FILEOFF_NULL, SIZE_DEFAULT);
  }
}

// returns idx of element that is larger or equals or arr length if larger
static size_t index_slot(const size_t entry_size, const size_t slot_len,
                         const struct slot_page_entry entries[]) {

  int l = 0, r = slot_len;
  while (l <= r) {
    int med = (l + r) / 2;
    if (entries[med].slot_size < entry_size) {
      l = med + 1;
    } else if (entries[med].slot_size > entry_size) {
      r = med - 1;
    } else {
      return med;
    }
  }
  return l;
}

static inline size_t max_slot_size(const size_t slot_len,
                                   const struct slot_page_entry entries[]) {
  return entries[slot_len - 1].slot_size;
}

static fileoff_t slot_page_create(struct dbms *dbms, struct slot_page_entry *entry,
                                  slot_page **sp_out) {
  pageoff_t page_size = sp_size(entry->slot_size, entry->slot_count);
  page_entry new = dbms_page_malloc(dbms, page_size);
  slot_page *page = sp_construct_slot_init(entry->slot_size, entry->slot_count);
  sp_create(page, dbms->dbfile->file, new.start);
  *sp_out = page;
  return new.start;
}

// find in data distribution or malloc it using pagealloc
static void slot_page_select_pop_single(struct dbms *dbms,
                                        struct slot_page_entry *entry,
                                        page_entry *pe_out, slot_page **sp_out) {
  FILE *file = dbms->dbfile->file;
  page_container *last = dbms_container_open(dbms, entry->last);
  page_entry last_cont = (page_entry){.start = entry->last, last->header.base.size};

  const fileoff_t prev_loc = last->header.prev;

  if (container_empty(last)) {
    // unload last page
    container_destruct(&last);

    if (!fileoff_is_null(prev_loc)) {
      // set last page pos to previous
      entry->last = prev_loc;
      page_container *prev = dbms_container_open(dbms, prev_loc);

      *pe_out = *container_pop(prev);
      *sp_out = sp_construct(pe_out->size);
      sp_select(*sp_out, file, pe_out->start);
      // write and close resources
      container_alter_destruct(&prev, file, prev_loc);
      // free last page
      dbms_page_free(dbms, &last_cont);
    } else {
      pe_out->start = slot_page_create(dbms, entry, sp_out);
      pe_out->size = (*sp_out)->header.base.size;
    }
  } else {
    *pe_out = *container_pop(last);
    *sp_out = sp_construct(pe_out->size);
    sp_select(*sp_out, file, pe_out->start);

    container_alter_destruct(&last, file, last_cont.start);
  }
}

static void force_container_push(struct dbms *dbms, struct slot_page_entry *entry,
                                 page_entry *pe_in) {
  page_container *last = dbms_container_open(dbms, entry->last);

  if (container_full(last)) {
    container_destruct(&last);
    entry->last = dbms_container_create(dbms, SIZE_DEFAULT, entry->last, &last);
  }
  container_push(last, pe_in);
  container_alter_destruct(&last, dbms->dbfile->file, entry->last);
}

// push page inside container if it is not full
static void slot_page_deselect(struct dbms *dbms, struct slot_page_entry *entry,
                               page_entry *pe_in, slot_page **sp_in) {
  // if slot_page is not full
  if (!sp_full(*sp_in)) {
    force_container_push(dbms, entry, pe_in);
  }
  // write and unload from ram
  sp_alter(*sp_in, dbms->dbfile->file, pe_in->start);
  sp_destruct(sp_in);
}

// @return - po_ptr where data was inserted
static po_ptr insert_once(struct dbms *dbms, const void *data, const size_t size) {
  meta *meta = dbms->meta;
  size_t slot_index = index_slot(size, meta->slot_len, meta->slot_entries);
  struct slot_page_entry *slot_entry = &meta->slot_entries[slot_index];
  slot_page *sp;
  page_entry sp_entry;

  po_ptr returned;

  slot_page_select_pop_single(dbms, slot_entry, &sp_entry, &sp);
  returned.offset = sp_insert_data(sp, data, size);
  returned.fileoff = sp_entry.start;
  slot_page_deselect(dbms, slot_entry, &sp_entry, &sp);

  return returned;
}

po_ptr dbms_insert_data(struct dbms *dbms, const void *data, const size_t size) {
  po_ptr po_start;
  meta *meta = dbms->meta;

  size_t mxs_size = max_slot_size(meta->slot_len, meta->slot_entries);
  assert(mxs_size > sizeof(po_ptr));

  const size_t slots_with_ptr =
      (size - sizeof(po_ptr) - 1) / (mxs_size - sizeof(po_ptr));

  const size_t mxs_sub = mxs_size - sizeof(po_ptr);

  void *cur_data_ptr = (void *)data + mxs_sub * slots_with_ptr;
  size_t cur_size_ptr = size - mxs_sub * slots_with_ptr;

  po_ptr next = insert_once(dbms, cur_data_ptr, cur_size_ptr);
  po_start = next;

  // [-------A]->[-------A]->[---------]
  if (slots_with_ptr > 0) {
    void *buffer = malloc(mxs_size);
    while (cur_size_ptr < size) {
      cur_size_ptr += mxs_sub;
      cur_data_ptr += mxs_sub;

      memcpy(buffer, cur_data_ptr, mxs_size - sizeof(po_ptr));
      memcpy(buffer + mxs_sub, &next, sizeof(po_ptr));
      next = insert_once(dbms, cur_data_ptr, mxs_size);
    }
    free(buffer);
  }
  return po_start;
}

// size - is needed to track pages with size > mx_slot_size
void dbms_remove_data(struct dbms *dbms, po_ptr po_cur, const size_t size) {
  FILE *file = dbms->dbfile->file;
  meta *meta = dbms->meta;

  slot_page *cur = sp_construct_select(file, po_cur.fileoff);
  // mxs is used as max size to check if data continues in another slot
  po_ptr po_next;
  size_t slot_size;

  for (size_t cur_size = 0; cur_size < size; cur_size += slot_size) {
    slot_size = cur->header.slot_size;
    size_t po_slot_size = slot_size - sizeof(po_ptr);

    bool has_next = (size - cur_size > slot_size);

    if (has_next) {
      pageoff_t offset = get_pageoff_t(po_cur.offset.bytes + po_slot_size);
      po_next = *(po_ptr *)sp_select_data(cur, offset);
    }

    size_t idx = index_slot(cur->header.slot_size, meta->slot_len, meta->slot_entries);
    struct slot_page_entry sp_entry = meta->slot_entries[idx];
    page_entry entry =
        (page_entry){.size = cur->header.base.size, .start = po_cur.fileoff};

    // get element to next page
    bool sp_was_full = sp_full(cur);
    // pop
    sp_remove_data(cur, po_cur.offset);
    // add to datadistr
    if (sp_was_full) {
      force_container_push(dbms, &sp_entry, &entry);
    }
    // write and unload from ram
    sp_alter_destruct(&cur, file, po_cur.fileoff);

    // iterate next
    if (has_next) {
      cur = sp_construct_select(file, po_next.fileoff);
      po_cur = po_next;
    } else {
      break;// return from loop after last element is processed
    }
  }
}

// also can be like iterator
void dbms_select_data(struct dbms *dbms, po_ptr po_cur, const size_t size,
                      void *data_out) {
  FILE *file = dbms->dbfile->file;

  slot_page *cur = sp_construct_select(file, po_cur.fileoff);

  po_ptr po_next;
  size_t slot_size;

  assert(size > 0);
  for (size_t cur_size = 0; cur_size < size; cur_size += slot_size) {
    slot_size = cur->header.slot_size;
    bool has_next = (size - cur_size > slot_size);

    void *data = sp_select_data(cur, po_cur.offset);

    if (has_next) {
      size_t po_slot_size = slot_size - sizeof(po_ptr);
      memcpy(data_out, data, po_slot_size);
      // next page
      pageoff_t offset = get_pageoff_t(po_cur.offset.bytes + po_slot_size);
      po_next = *(po_ptr *)sp_select_data(cur, offset);
    } else {
      memcpy(data_out, data, slot_size);
    }

    sp_destruct(&cur);

    // iterate next
    if (has_next) {// always true unless its is last node
      cur = sp_construct_select(file, po_next.fileoff);
      po_cur = po_next;
    } else {
      break;// return from loop after last element is processed
    }
  }
}
