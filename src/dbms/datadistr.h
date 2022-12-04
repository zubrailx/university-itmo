#pragma once

// distribute data into special pages (insertion and deletion with O(n))
// proper handling of insertions of data of different size like malloc
//
// it finds the best suit size and inserts string there
// also handles empty pages like page allocator but at the level of page, not document
struct dbms;

void dbms_dd_create_close(struct dbms *dbms);
