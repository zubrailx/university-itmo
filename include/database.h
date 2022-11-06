#pragma once

#include <stdbool.h>

typedef struct database database;

bool database_is_writable(const database *db);
bool database_is_opened(const database *db);
const char *database_get_name(const database *db);

database *database_create(const char *filename);
database *database_open(const char *filename, bool is_writable);
void database_flush(const database *database);
void database_close(database **database);
void database_remove(database **database);
