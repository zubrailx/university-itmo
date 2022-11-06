#pragma once

typedef struct database database;

database *database_create(const char *filename);
database *database_open(const char *filename);
void database_flush(const database *database);
void database_close(database **database);
void database_remove(database **database);
