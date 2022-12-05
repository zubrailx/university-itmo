#include "printers.hpp"

void print_database_page(database_page *page) {
  printf("----------------------\n");
  printf("database_page\n");
  printf("size: %u\n", page->header.base.size.bytes);
  printf("type: %hhu\n", page->header.base.type);
  printf("index_start: %u\n", page->header.index_start.bytes);
  printf("typle_end: %u\n", page->header.tuple_end.bytes);
  printf("next: %zu\n", page->header.next.bytes);
  printf("prev: %zu\n", page->header.prev.bytes);
  printf("----------------------\n");
}

void print_database_typle(dp_tuple *typle) {
  printf("----------------------\n");
  printf("database_typle\n");
  printf("cols: %zu\n", typle->header.cols);
  printf("is_present: %u\n", typle->header.is_present);
  printf("first_page: %zu\n", typle->header.tp_first.bytes);
  printf("last_page: %zu\n", typle->header.tp_last.bytes);
  printf("gappy_page: %zu\n", typle->header.gappy_last.bytes);
  printf("not_inline: %d\n", (int)typle->header.sso.not_inline);
  printf("----------------------\n");
}

// void print_database_tables(dbms *dbms) {
//   dp_iter *iter = dp_iter_construct(dbms);
//   dp_typle *typle = dp_iter_get(iter);
//   while (typle) {
//     print_database_typle(typle);
//
//     table_page *tp = dbms_tp_select(dbms, typle->header.first);
//     print_table_page(tp);
//     tp_destruct(&tp);

//     dp_iter_next(iter);
//     typle = dp_iter_get(iter);
//   }
//   dp_iter_destruct(&iter);
// }

// void print_table_page(table_page *page) {
//   printf("----------------------\n");
//   printf("table_page\n");
//   printf("size: %u\n", page->header.base.size.bytes);
//   printf("type: %hhu\n", page->header.base.type);
//   printf("next: %zu\n", page->header.next.bytes);
//   printf("prev: %zu\n", page->header.prev.bytes);
//   printf("typle_end: %u\n", page->header.typle_end.bytes);
//   printf("hole_start: %u\n", page->header.hole_start.bytes);
//   printf("typle_limit: %u\n", page->header.typle_limit.bytes);
//   printf("----------------------\n");
// }
