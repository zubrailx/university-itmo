#include "sections/table.h"
#include "util.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void database_table_unload(DTTyple **dttyple) {
  free(*dttyple);
  *dttyple = NULL;
}

// database_table is allocated with name and columns
// DTTypleInline *database_table_inline_load(Database *database,
//                                           DatabaseSection *section,
//                                           sectoff_t off_index) {
//   DTIndex *index = section->body + database_section_get_body_offset(off_index);
//   sectoff_t offbody_start = database_section_get_body_offset(index->off_start);
//   sectoff_t offbody_end = database_section_get_body_offset(index->off_end);
//   size_t typle_size = 0;
//   // Load table name
//   DTTyple *typle = section->body + offbody_start;
//   char *table_name;
//   // Add size of table name + info
//   typle_size += sizeof(DTTypleInline);
//   if (typle->is_inline) {
//     table_name = strdup(typle->ptrname.name_start);
//   } else {
//     // if string in current section
//     fileoff_t sect_address = typle->ptrname.pointer.sect_address;
//     fileoff_t sect_offset = typle->ptrname.pointer.offset;
//     if (sect_address == SECTION_OFFSET_NULL) {
//       table_name = strdup(typle->ptrname.name_start);
//     } else {
//       void *base_loaded = section_load(database, sect_address);
//       table_name = strdup(base_loaded + sect_offset);
//       section_unload((BaseSection **)&base_loaded);
//     }
//   }
//   typle_size += strlen(table_name) + 1; // + null terminator

// }
// // Return NULL is exists
// DTTyple *database_table_create(Database *database, const char *name) {
//   assert(database->is_opened);
//   DTTyple *selected = database_table_select(database, name);
//   if (selected != NULL) {
//     free(selected);
//     return NULL;
//   }
// }

// DTTyple *database_table_select(Database *database, const char *name) {
//   assert(database->is_opened);
//   DatabaseSection *ds = database_section_load(database, database->ds.ds_first);
//   TableSection *table_section = NULL;
//   while (ds != NULL) {
//     sectoff_t offbody_start = 0;
//     sectoff_t offbody_end =
//         database_section_get_body_offset(ds->header->off_index_last);
//     // Check each DTIndex
//     for (; offbody_start < offbody_end && table_section == NULL;
//          offbody_start += sizeof(DTIndex)) {
//       BaseSection *sect_loaded = NULL;
//       const char *stored_name;

//       DTIndex *index = (void *)ds->body + offbody_start;
//       if (index->is_cleared) {
//         continue;
//       }
//       DTTyple *typle =
//           (void *)ds->body + database_section_get_body_offset(index->off_start);
//       union DTTName *ptrname = (void *)typle + offsetof(DTTyple, ptrname);
//       // Get stored_name from DTTyple
//       if (typle->is_inline) {
//         stored_name = ptrname->name_start;
//       } else {
//         SOPointer ptr = ptrname->pointer;
//         // if string is stored in current page
//         if (ptr.sect_address == SECTION_OFFSET_NULL) {
//           stored_name =
//               (void *)ds->body + database_section_get_body_offset(ptr.offset);
//         } else {
//           // TODO: Add section bufferization
//           sect_loaded = section_load(database, ptr.sect_address);
//           stored_name = (void *)sect_loaded + ptr.offset;
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

// DTTyple *database_table_delete(Database *database, const char *name) {}
