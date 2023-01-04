#include <database.h>
#include <schema.h>
#include <dto_table.h>
#include <dto_row.h>
#include <schema.h>
#include <table.h>
#include <unistd.h>

int main() {
  const char *fname = ".db-very-large.db";
  struct dbms *dbms;
  if (access(fname, F_OK) == 0) {
    dbms = dbms_open(fname);
  } else {
    dbms = dbms_create(fname);
  }

  struct dto_table *t1 = dto_table_construct((char *)"table1");
  dto_table_add_column(t1, (char *)"name", DTO_COLUMN_STRING,
                       (dto_table_column_limits){.is_null = true, .is_unique = false});

  table_create(dbms, t1);
  dto_table_destruct(&t1);

  const char *e1 = "nnikitadksjfsajfkfjsafjlsajfsjaklfjlfjsaklnikitadksjfsajfkfjsafjls";
  double e2 = 100;

  const void *r1[] = {e1, &e2};
  struct dto_row_list l1 = dto_row_list_construct();
  dto_row_list_append(&l1, r1);

  for (size_t i = 0; i < 1000000; ++i) {
    row_list_insert(dbms, "table1", &l1);
  }
  dto_row_list_destruct(&l1);

  dbms_close(&dbms);
}
