#include "../dto/dto_table.h"
#include "../io/page/column_types.h"
#include "../io/page/p_database.h"

enum table_column_type column_type_to_page(enum dto_table_column_type from);

void column_limits_to_page(struct dto_table_column_limits *from,
                           struct dpt_col_limits *to);
