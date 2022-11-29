#pragma once

#include "io/page/sso.h"

struct dbms;

struct data_page *dbms_get_sso_string(char **input_ptr, page_sso *sso,
                                      struct dbms *dbms);
void dbms_insert_sso(page_sso *sso_out, const char *name, struct dbms *dbms);
