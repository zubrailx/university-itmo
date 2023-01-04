#include "p_container.h"

PAGE_DEFAULT_SELECT_IMPL(struct page_container, container)
PAGE_DEFAULT_CREATE_IMPL(struct page_container, container)
PAGE_DEFAULT_ALTER_IMPL(struct page_container, container)
PAGE_DEFAULT_DROP_IMPL(struct page_container, container)

PAGE_DEFAULT_CONSTRUCT_SELECT_IMPL(struct page_container, container)
PAGE_DEFAULT_ALTER_DESTRUCT_IMPL(struct page_container, container)
