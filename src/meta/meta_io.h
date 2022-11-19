#include <database.h>
#include "../util.h"

my_defstruct(database_meta);

void meta_select(database_meta *meta, const database *db);
void meta_alter(const database_meta *meta, database *db);
void meta_create(const database_meta *meta, database *db);
