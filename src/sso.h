#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "po_ptr.h"
#include "util.h"

#define SSO_SSIZE_SIZE (sizeof(size_t) - sizeof(bool))
#define SSO_MXLEN (sizeof(STRUCT_STR_NOT_IN))

/* Struct that stored if string is not inlined
 * @ssize string size
 * @ptr pointer to string in different page
 */
#define STRUCT_STR_NOT_IN                                                              \
  struct {                                                                             \
    char ssize[SSO_SSIZE_SIZE];                                                        \
    struct po_ptr po_ptr;                                                              \
  } __attribute__((packed))

my_defstruct(page_sso);
struct page_sso {
  bool is_inline;
  union {
    STRUCT_STR_NOT_IN;
    char name[SSO_MXLEN];
  };
} __attribute__((packed));

size_t sso_to_size(const char *ssize);
void size_to_sso(size_t size, char *ssize);
