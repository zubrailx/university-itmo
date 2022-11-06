#include "meta.h"

#include <database.h>

database_meta *meta_load(const database *db);
void meta_unload(database_meta **meta);

void meta_alter(const database *db);
database_meta *meta_create(const database *db, const char* meta_info);

