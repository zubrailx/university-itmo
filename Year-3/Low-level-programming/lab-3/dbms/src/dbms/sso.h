#pragma once

#include <dbms/internals/sso.h>

struct dbms;

// struct data_page *dbms_construct_select_sso(char **input_ptr, page_sso *sso,
//                                       struct dbms *dbms);

void *dbms_sso_construct_select(const page_sso *sso, struct dbms *dbms);
page_sso dbms_sso_insert(size_t size, const void *data, struct dbms *dbms);
void dbms_sso_remove(const page_sso *sso, struct dbms *dbms);
