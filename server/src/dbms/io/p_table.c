#include "p_table.h"

PAGE_DEFAULT_SELECT_IMPL(struct table_page, tp)
PAGE_DEFAULT_CREATE_IMPL(struct table_page, tp)
PAGE_DEFAULT_ALTER_IMPL(struct table_page, tp)
PAGE_DEFAULT_DROP_IMPL(struct table_page, tp)

PAGE_DEFAULT_CONSTRUCT_SELECT_IMPL(struct table_page, tp)
PAGE_DEFAULT_ALTER_DESTRUCT_IMPL(struct table_page, tp)
