#include "../dto/dto_table.h"
#include "../io/page/p_database.h"
#include "../io/page/column_types.h"

enum table_column_type table_column_type_to_page(const enum dto_table_column_type from);

void table_column_limits_to_page(struct dto_table_column_limits *from,
                                 struct dpt_col_limits *to);
