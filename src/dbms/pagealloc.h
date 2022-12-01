#pragma once

struct dbms;

#include "io/page/p_container.h"

// pa - page alloc

void dbms_pa_create_close(struct dbms *dbms, const fileoff_t prev);
page_entry dbms_pa_malloc(struct dbms *dbms, const pageoff_t size);
void dbms_pa_free(struct dbms *dbms, const page_entry *entry);
