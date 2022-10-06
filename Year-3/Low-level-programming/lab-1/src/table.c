// #include "sections/table.h"
// #include "util.h"

// #include <assert.h>
// #include <malloc.h>

// sectoff_t TABLE_SECTION_SIZE = 4096;

// TableSection *table_section_load(Database *database, const fileoff_t offset)
// {
//   void *section = section_load(database, offset);
//   TableSection *ts = my_malloc(TableSection);
//   ts->header = (TableHeader *)section;
//   ts->body = section + sizeof(TableHeader);
//   return ts;
// }

// void table_section_unload(TableSection **ts_ptr) {
//   TableSection *ts = *ts_ptr;
//   free((void *)ts->header);
//   free(ts);
//   *ts_ptr = NULL;
// }

// If found already exists -> return NULL or return new created table
// TableSection *table_section_create(Database *database, DTTypleInline
// *dttinline) {
//   assert(database->is_opened);
//   TableSection *selected = table_section_select(database, dttinline->name);
//   if (selected != NULL) {
//     table_section_unload(&selected);
//     return NULL;
//   }
//   return table_section_create(database, name);
// }

// TableSection *table_section_delete(Database *database, const char *name) {
//   assert(database->is_opened);
//   TableSection *selected = table_delete(database, name);
//   if (selected == NULL) {
//     return NULL;
//   }
// }

// TableSection *table_section_select(Database *database, const char *name) {
//   assert(database->is_opened);
//   DatabaseSection *ds = database_section_load(database,
//   database->ds.ds_first); TableSection *table_section = NULL; while (ds !=
//   NULL) {
//     sectoff_t offbody_start = 0;
//     sectoff_t offbody_end =
//         database_section_get_body_offset(ds->header->off_index_last);
//     // Check each DTIndex
//     for (; offbody_start < offbody_end && table_section == NULL;
//          offbody_start += sizeof(DTIndex)) {
//       DatabaseSection *ds_loaded = NULL;
//       const char *stored_name;

//       DTIndex *index = (void *)ds->body + offbody_start;
//       if (index->is_cleared) {
//         continue;
//       }
//       DTTyple *typle =
//           (void *)ds->body +
//           database_section_get_body_offset(index->off_start);
//       union DTTName *ptrname = (void *)typle + offsetof(DTTyple, ptrname);
//       // Get stored_name from DTTyple
//       if (typle->is_inline) {
//         stored_name = ptrname->name_start;
//       } else {
//         SOPointer ptr = ptrname->pointer;
//         // if string is stored in current page
//         if (ptr.page_address == SECTION_OFFSET_NULL) {
//           stored_name =
//               (void *)ds->body +
//               database_section_get_body_offset(ptr.offset);
//         } else {
//           // TODO: Add section bufferization
//           ds_loaded = database_section_load(database, ptr.page_address);
//           stored_name = (void *)ds_loaded->body +
//                         database_section_get_body_offset(ptr.offset);
//         }
//       }
//       // Compare and close resources
//       if (strcmp(name, stored_name) == 0) {
//         table_section = table_section_load(database, typle->fileoff);
//       }
//       if (ds_loaded) {
//         database_section_unload(&ds_loaded);
//       }
//     }
//     // Close ds if found or unload current and load next else
//     if (table_section != NULL) {
//       database_section_unload(&ds);
//       return table_section;
//     } else {
//       DatabaseSection *next = database_section_load_next(database, ds);
//       database_section_unload(&ds);
//       ds = next;
//     }
//   }
//   return NULL;
// }

// void database_table_delete(Database * database, const char *name) {}
