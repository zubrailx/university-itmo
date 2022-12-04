#include "op_sso.h"

#include "datadistr.h"
#include <malloc.h>
#include <string.h>

// TODO: implement after i can get the idea of usage
void *dbms_construct_select_sso(page_sso *sso, struct dbms *dbms) {
  void *data;
  if (sso->not_inline) {
    size_t size = sso_to_size(sso->ssize);
    data = malloc(size);
    dbms_select_data(dbms, sso->po_ptr, size, data);
    return data;
  } else {
    data = strdup(sso->name);
  }
  return data;
}

// insert sso string in data page or only store in page_sso
// void dbms_insert_sso(page_sso *sso_out, const void *data, struct dbms *dbms) {
//   dbms_insert_data(dbms, data, )
// }

// void dbms_remove_sso(page_sso *sso_out, const char)
