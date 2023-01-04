#pragma once

struct dbms;

#include "io/page/p_container.h"

// pa - page alloc

void dbms_pa_create_close(struct dbms *dbms, fileoff_t prev);
page_entry dbms_page_malloc(struct dbms *dbms, pageoff_t size);
void dbms_page_free(struct dbms *dbms, const page_entry *entry);
