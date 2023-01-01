#pragma once

struct dbms *dbms_create(const char *fname);
struct dbms *dbms_open(const char *fname);
void dbms_close(struct dbms **dbms_ptr);
void dbms_remove(struct dbms **dbms_ptr);
