#pragma once

#include "p_base.h"
#include "page/p_table.h"

PAGE_DEFAULT_SELECT(struct table_page, tp)
PAGE_DEFAULT_CREATE(struct table_page, tp)
PAGE_DEFAULT_ALTER(struct table_page, tp)
PAGE_DEFAULT_DROP(struct table_page, tp)

PAGE_DEFAULT_CONSTRUCT_SELECT(struct table_page, tp)
PAGE_DEFAULT_ALTER_DESTRUCT(struct table_page, tp)
