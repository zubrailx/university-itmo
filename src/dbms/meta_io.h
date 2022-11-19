#pragma once

#include <database.h>

#include "dbfile.h"
#include "../util/define.h"

struct dbmeta;

void meta_load(struct dbmeta *meta, dbfile *db);

void meta_alter(const struct dbmeta *meta, dbfile *db);
void meta_create(const struct dbmeta *meta, dbfile *db);
