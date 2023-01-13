#pragma once

// distribute data into special pages (insertion and deletion with O(1))
// proper handling of insertions of data of different size like malloc
//
// it finds the best suit size and inserts string there
// also handles empty pages like page allocator but at the level of page, not document

// strings are stored in s

#include <dbms/internals/po_ptr.h>
#include <stddef.h>

struct dbms;

void dbms_dd_create_close(struct dbms *dbms);

po_ptr dbms_dd_insert_data(struct dbms *dbms, const void *data, size_t size);
void dbms_dd_remove_data(struct dbms *dbms, po_ptr po_cur, size_t size);
void dbms_dd_select_data(struct dbms *dbms, po_ptr po_cur, size_t size, void *data_out);
