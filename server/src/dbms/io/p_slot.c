#include "p_slot.h"

PAGE_DEFAULT_SELECT_IMPL(struct slot_page, sp)
PAGE_DEFAULT_CREATE_IMPL(struct slot_page, sp)
PAGE_DEFAULT_ALTER_IMPL(struct slot_page, sp)
PAGE_DEFAULT_DROP_IMPL(struct slot_page, sp)

PAGE_DEFAULT_CONSTRUCT_SELECT_IMPL(struct slot_page, sp)
PAGE_DEFAULT_ALTER_DESTRUCT_IMPL(struct slot_page, sp)
