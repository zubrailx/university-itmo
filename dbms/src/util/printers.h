#pragma once

#include <dbms/util/printers.h>

struct table_page;
struct database_page;

void print_database_page(struct database_page *page);
void print_table_page(struct table_page *page);
