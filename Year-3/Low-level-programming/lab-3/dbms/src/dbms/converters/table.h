#include "../io/page/p_database.h"
#include <dbms/dto_table.h>

struct dbms;

void column_limits_to_page(struct dto_table_column_limits *from,
                           struct dpt_col_limits *to);

void column_limits_to_dto(struct dpt_col_limits *from, struct dto_table_column_limits *to);

