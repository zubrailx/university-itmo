#include "database.h"

uint32_t DATABASE_PAGE_SIZE = 1024;

PAGE_DEFAULT_CREATE_IMPL(struct database_page, dp)
PAGE_DEFAULT_ALTER_IMPL(struct database_page, dp)
PAGE_DEFAULT_DROP_IMPL(struct database_page, dp)
