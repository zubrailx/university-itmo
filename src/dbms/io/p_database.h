#pragma once

#include "p_base.h"
#include "page/p_database.h"

struct dbmeta;

PAGE_DEFAULT_LOAD(struct database_page, dp)
PAGE_DEFAULT_CREATE(struct database_page, dp)
PAGE_DEFAULT_ALTER(struct database_page, dp)
PAGE_DEFAULT_DROP(struct database_page, dp)
