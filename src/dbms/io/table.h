#pragma once

#include "base.h"
#include "page/table.h"

struct dbmeta;

PAGE_DEFAULT_LOAD(struct table_page, tp)
PAGE_DEFAULT_CREATE(struct table_page, tp)
PAGE_DEFAULT_ALTER(struct table_page, tp)
PAGE_DEFAULT_DROP(struct table_page, tp)
