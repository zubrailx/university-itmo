#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "converters/table_typle.h"
#include "core/dbmeta.h"
#include "op_sso.h"
#include "page.h"

// create typle and insert sso strings
// static struct dp_typle *_create_typle(const struct dto_table *dto_table,
//                                       struct dbms *dbms, size_t *typle_size_out) {
//   // Convert dto to entity
//   dpt_header header = {};
//   header.cols = dto_table_columns(dto_table);
//   dbms_insert_sso(&header.sso, dto_table->name, dbms);

//   // construct dptyple
//   size_t typle_size = dp_typle_size(header.cols);
//   dp_typle *typle = calloc(typle_size, 1);
//   typle->header = header;

//   dto_table_column *dto_col = dto_table->first;
//   dpt_column col = {};
//   for (size_t i = 0; i < typle->header.cols; ++i) {
//     // convert dto column to entity
//     table_column_limits_to_page(&dto_col->lims, &col.limits);
//     col.type = table_column_type_to_page(dto_col->type);
//     dbms_insert_sso(&col.sso, dto_col->name, dbms);

//     typle->columns[i] = col;
//     dto_col = dto_col->next;
//   }
//   *typle_size_out = typle_size;
//   return typle;
// }

// create pages needed
// static void _create_typle_init(struct dp_typle *typle, struct dbms *dbms) {
  // typle->header.first = dbms_tp_create_close(dbms, SIZE_DEFAULT, FILEOFF_NULL, typle);
  // typle->header.last = typle->header.first;
  // // create container and add page inside it 
  // page_container *cont;
  // typle->header.gappy_last = dbms_container_create(dbms, SIZE_DEFAULT, &cont);
  // struct page_entry entry = page_entry_construct(typle->header.first);
  // container_push(cont, &entry);
  // dbms_container_close(&cont, typle->header.gappy_last, dbms);
// }

// Just insert table, no checks for existance
// void dbms_create_table(const struct dto_table *dto_table, struct dbms *dbms) {
//   size_t typle_size;
//   dp_typle *typle = _create_typle(dto_table, dbms, &typle_size);
//   _create_typle_init(typle, dbms);

//   // select last page and insert there or create new page
//   fileoff_t page_loc = dbms->meta->dp_last;
//   database_page *page = dbms_dp_select(dbms, page_loc);

//   if (dp_insert_typle(page, typle).bytes == 0) {
//     dp_destruct(&page);

//     pageoff_t size = dp_body_page(get_bodyoff_t(typle_size + sizeof(page_index)));
//     page_loc = dbms_dp_create(dbms, size, &page);

//     assert(dp_insert_typle(page, typle).bytes > 0);
//   }
//   // close resources
//   dbms_dp_close(&page, page_loc, dbms);
//   free(typle);
// }

// void _dbms_drop_table_pages(const dp_typle *typle, struct dbms *dbms) {
//   fileoff_t current = typle->header.first;
//   // drop all table pages
//   while (!fileoff_is_null(current)) {
//     database_page *page = dbms_dp_select(dbms, current);
//     dbms_page_drop(dbms, current);
//     current.bytes = page->header.next.bytes;
//     dp_destruct(&page);
//   }
//   // drop gappy pages
//   current = typle->header.gappy_last;
//   while (!fileoff_is_null(current)) {
//     page_container *page = dbms_container_select(dbms, current);
//     dbms_page_drop(dbms, current);
//     current.bytes = page->header.prev.bytes;
//     container_destruct(&page);
//   }
// }

// // return typle inside database_page
// dp_typle *_dbms_select_typle(const fileoff_t fileoff, const pageoff_t idx_pageoff,
//                              struct dbms *dbms, database_page **page_out) {
//   database_page *page = dbms_dp_select(dbms, fileoff);
//   dp_typle *typle = dp_typle_locate(page, idx_pageoff);
//   *page_out = page;
//   return typle;
// }

// bool dbms_drop_table(const fileoff_t fileoff, const pageoff_t idx_pageoff,
//                      struct dbms *dbms) {
//   database_page *page;
//   dp_typle *typle = _dbms_select_typle(fileoff, idx_pageoff, dbms, &page);

//   _dbms_drop_table_pages(typle, dbms);

//   bool res = dp_drop_table(page, idx_pageoff);

//   dbms_dp_close(&page, fileoff, dbms);
//   return res;
// }
