extern "C" {
#include "../src/dbms/core/dbfile.h"
#include "../src/dbms/core/dbmeta.h"
#include "../src/dbms/core/dbms.h"
#include "../src/dbms/dto/dto_table.h"
#include "../src/dbms/page.h"
#include "../src/dbms/iter.h"
#include "../src/table.h"
#include "database.h"
#include <database.h>
}

void print_database_page(database_page *page);
// void print_database_typle(dp_typle *typle);
void print_database_tables(dbms *dbms);
void print_table_page(table_page *page);
void print_free_pages(dbms *dbms);
