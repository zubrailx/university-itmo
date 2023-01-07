#pragma once

#include <qpg/ast.hpp>

extern "C" {
#include <dbms/dto_table.h>
#include <dbms/util/column_types.h>
}

table_column_type toTableColumnType(DataType dtype);

std::string toString(table_column_type type);
