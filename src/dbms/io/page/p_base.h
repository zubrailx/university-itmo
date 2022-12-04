#pragma once

#include <inttypes.h>

#include <util/internals.h>

#define _PAGE_PREFIX page
#define _PAGE_TYPE struct base_page

typedef struct base_header {
  uint8_t type;
  struct pageoff_t size;
} base_header;

typedef _PAGE_TYPE { struct base_header header; }
__attribute__((packed)) base_page;

enum page_types {
  PAGE_UNKNOWN = 0,
  PAGE_CONTAINER,
  PAGE_DATABASE,
  PAGE_SLOT,
  PAGE_TABLE,
  PAGE_DUMPED,
};

_PAGE_TYPE *page_construct(const struct pageoff_t size, enum page_types type);
void page_destruct(_PAGE_TYPE **page_ptr);

// Constructors / destructors
#define PAGE_CONSTRUCT_DEFAULT(page_type, prefix)                                      \
  page_type *prefix##_construct(struct pageoff_t size);

#define PAGE_CONSTRUCT_DEFAULT_IMPL(page_type, prefix, ENUM_PAGE)                      \
  page_type *prefix##_construct(struct pageoff_t size) {                               \
    return (page_type *)page_construct(size, ENUM_PAGE);                               \
  }

#define PAGE_DESTRUCT_DEFAULT(page_type, prefix)                                       \
  void prefix##_destruct(page_type **page_ptr);

#define PAGE_DESTRUCT_DEFAULT_IMPL(page_type, prefix)                                  \
  void prefix##_destruct(page_type **page_ptr) {                                       \
    return page_destruct((base_page **)page_ptr);                                      \
  }

// Fileoff / pageoff
#define INLINE_BODYOFF_TO_PAGEOFF(page_type, m_body, prefix)                           \
  inline pageoff_t prefix##_body_page(bodyoff_t bodyoff) {                             \
    return get_pageoff_t(bodyoff.bytes + offsetof(page_type, m_body));                 \
  }
#define EXTERN_INLINE_BODYOFF_TO_PAGEOFF(page_type, m_body, prefix)                    \
  extern inline pageoff_t prefix##_body_page(bodyoff_t bodyoff);

#define INLINE_PAGEOFF_TO_BODYOFF(page_type, m_body, prefix)                           \
  inline bodyoff_t prefix##_page_body(pageoff_t pageoff) {                             \
    return get_bodyoff_t(pageoff.bytes - offsetof(page_type, m_body));                 \
  }
#define EXTERN_INLINE_PAGEOFF_TO_BODYOFF(page_type, m_body, prefix)                    \
  extern inline bodyoff_t prefix##_page_body(pageoff_t pageoff);

#undef _PAGE_TYPE
#undef _PAGE_PREFIX
