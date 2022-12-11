#include "../dbms/core/dbfile.h"
#include "../dbms/core/dbmeta.h"
#include "../dbms/core/dbms.h"
#include "../dbms/dto/dto_table.h"
#include "../dbms/iter.h"
#include "../dbms/page.h"

#include "../dbms/op_schema.h"
#include "../table.h"

void print_database_page(database_page *page);
// void print_database_typle(dp_typle *typle);
void print_database_tables(dbms *dbms);
void print_table_page(table_page *page);
void print_free_pages(dbms *dbms);

void print_table_tuple(const tp_tuple *tuple, const dp_tuple *dpt,
                       const tpt_col_info *col_info);
void print_table_rows(dbms *dbms, const char *table_name);
