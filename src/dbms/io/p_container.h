#pragma once

#include "p_base.h"
#include "page/p_container.h"

PAGE_DEFAULT_SELECT(struct page_container, container)
PAGE_DEFAULT_CREATE(struct page_container, container)
PAGE_DEFAULT_ALTER(struct page_container, container)
PAGE_DEFAULT_DROP(struct page_container, container)

PAGE_DEFAULT_CONSTRUCT_SELECT(struct page_container, container)
PAGE_DEFAULT_ALTER_DESTRUCT(struct page_container, container)
