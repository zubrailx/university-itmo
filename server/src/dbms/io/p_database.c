#include "p_database.h"

PAGE_DEFAULT_SELECT_IMPL(struct database_page, dp)
PAGE_DEFAULT_CREATE_IMPL(struct database_page, dp)
PAGE_DEFAULT_ALTER_IMPL(struct database_page, dp)
PAGE_DEFAULT_DROP_IMPL(struct database_page, dp)

PAGE_DEFAULT_CONSTRUCT_SELECT_IMPL(struct database_page, dp)
PAGE_DEFAULT_ALTER_DESTRUCT_IMPL(struct database_page, dp)
