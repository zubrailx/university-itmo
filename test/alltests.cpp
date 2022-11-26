#include "gtest/gtest.h"

extern "C" {
#include "../src/dbms/core/dbfile.h"
#include "../src/dbms/core/dbms.h"
#include "../src/dbms/core/meta.h"
#include "../src/dbms/dto/table.h"
#include "../src/dbms/iterator.h"
#include "../src/dbms/page.h"
#include "../src/table.h"
#include "database.h"
#include <database.h>
}

void print_database_page(database_page *page) {
  printf("------------------\n");
  printf("size: %u\n", page->header.base.size.bytes);
  printf("type: %hhu\n", page->header.base.type);
  printf("index_start: %u\n", page->header.index_start.bytes);
  printf("typle_end: %u\n", page->header.typle_end.bytes);
  printf("next: %zu\n", page->header.next.bytes);
  printf("prev: %zu\n", page->header.prev.bytes);
  printf("------------------\n");
}

void print_database_typle(dp_typle *typle) {
  printf("----------------------\n");
  printf("cols: %zu\n", typle->header.cols);
  printf("fileoff: %zu\n", typle->header.fileoff.bytes);
  printf("not_inline: %d\n", (int)typle->header.sso.not_inline);
  printf("----------------------\n");
}

int test(int argc, char **argv) {
  return 0;
}

int test_google(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

int main(int argc, char **argv) {
  // return test(argc, argv);
  return test_google(argc, argv);
}
