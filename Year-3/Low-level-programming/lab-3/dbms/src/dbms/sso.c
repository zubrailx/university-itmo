#include "sso.h"

#include "data_dist.h"
#include <malloc.h>
#include <string.h>

void *dbms_sso_construct_select(const page_sso *sso, struct dbms *dbms) {
  char *data;
  if (sso->not_inline) {
    size_t size = sso_to_size(sso->ssize);
    data = malloc(size);
    dbms_dd_select_data(dbms, sso->po_ptr, size, data);
    return data;
  } else {
    data = strdup(sso->name);
  }
  return data;
}

// @size - size of data + null terminator
page_sso dbms_sso_insert(const size_t size, const void *data, struct dbms *dbms) {
  page_sso res = {};
  res.not_inline = size > SSO_MXLEN;

  if (res.not_inline) {
    size_to_sso(size, res.ssize);
    res.po_ptr = dbms_dd_insert_data(dbms, data, size);
  } else {
    memcpy(res.name, data, size);
  }
  return res;
}

void dbms_sso_remove(const page_sso *sso, struct dbms *dbms) {
  if (sso->not_inline) {
    dbms_dd_remove_data(dbms, sso->po_ptr, sso_to_size(sso->ssize));
  }
}
