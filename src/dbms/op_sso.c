#include "op_sso.h"
#include "page.h"
#include <string.h>

// if data_page is not NULL then string was returned from
// data_page *dbms_get_sso_string(char **input_ptr, page_sso *sso, struct dbms *dbms) {
  // if (sso->not_inline) {
  //   data_page *da = dbms_da_select(dbms, sso->po_ptr.fileoff);
  //   *input_ptr = (void *)da + sso->po_ptr.offset.bytes;
  //   return da;
  // } else {
  //   *input_ptr = sso->name;
  //   return NULL;
  // }
// }

// insert sso string in data page or only store in page_sso
// void dbms_insert_sso(page_sso *sso_out, const char *name, dbms *dbms) {
  // size_t size = strlen(name);
  // sso_out->not_inline = size > SSO_MXLEN;
  // if (!sso_out->not_inline) {
  //   strcpy(sso_out->name, name);
  // } else {
  //   size_to_sso(size, sso_out->ssize);
  //   fileoff_t fileoff;
  //   pageoff_t pageoff;
  //   // dbms_da_insert_data(name, size, dbms, &fileoff, &pageoff);
  //   sso_out->po_ptr.fileoff = fileoff;
  //   sso_out->po_ptr.offset = pageoff;
  // }
// }
