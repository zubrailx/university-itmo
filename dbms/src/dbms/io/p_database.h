#pragma once

#include "p_base.h"
#include "page/p_database.h"

PAGE_DEFAULT_SELECT(struct database_page, dp)
PAGE_DEFAULT_CREATE(struct database_page, dp)
PAGE_DEFAULT_ALTER(struct database_page, dp)
PAGE_DEFAULT_DROP(struct database_page, dp)

PAGE_DEFAULT_CONSTRUCT_SELECT(struct database_page, dp)
PAGE_DEFAULT_ALTER_DESTRUCT(struct database_page, dp)
