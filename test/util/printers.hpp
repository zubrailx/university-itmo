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

void print_database_page(database_page *page);
void print_database_typle(dp_typle *typle);
