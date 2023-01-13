#pragma once

#include "p_base.h"
#include "page/p_slot.h"

PAGE_DEFAULT_SELECT(struct slot_page, sp)
PAGE_DEFAULT_CREATE(struct slot_page, sp)
PAGE_DEFAULT_ALTER(struct slot_page, sp)
PAGE_DEFAULT_DROP(struct slot_page, sp)

PAGE_DEFAULT_CONSTRUCT_SELECT(struct slot_page, sp)
PAGE_DEFAULT_ALTER_DESTRUCT(struct slot_page, sp)
