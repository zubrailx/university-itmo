#pragma once

#include "../dbms/io/page/p_base.h"
#include <stdio.h>

void print_page(base_page *page);
void print_page_full(base_page *page, fileoff_t addr);
