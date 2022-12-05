#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "po_ptr.h"

#define SSO_SSIZE_SIZE (sizeof(size_t) - sizeof(bool))
#define SSO_MXLEN (sizeof(struct _str_not_in))

/* Struct that stored if string is not inlined
 * @ssize string size
 * @ptr pointer to string in different page
 */
#define _STR_NOT_IN                                                                    \
  {                                                                                    \
    char ssize[SSO_SSIZE_SIZE];                                                        \
    struct po_ptr po_ptr;                                                              \
  }                                                                                    \
  __attribute__((packed))
// Create non-anonymous class for c++ compatibility
struct _str_not_in _STR_NOT_IN;

// Small string optimization struct with anonymous struct inside
typedef struct page_sso {
  union {
    struct _STR_NOT_IN;
    char name[SSO_MXLEN];
  };
  bool not_inline;// should be 0 if inline because string is null terminated
} __attribute__((packed)) page_sso;

size_t sso_to_size(const char *ssize);
void size_to_sso(size_t size, char *ssize);

#undef _STR_NOT_IN
