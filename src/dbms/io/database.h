#pragma once

#include "base.h"
#include "page/database.h"

struct dbmeta;

PAGE_DEFAULT_CREATE(struct database_page, dp)
PAGE_DEFAULT_ALTER(struct database_page, dp)
PAGE_DEFAULT_DROP(struct database_page, dp)
