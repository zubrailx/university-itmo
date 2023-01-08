#pragma once

#include <qpg/ast.hpp>

extern "C" {
#include <dbms/dto_table.h>
#include <dbms/util/column_types.h>
}

table_column_type toTableColumnType(DataType dtype);

std::string toString(table_column_type type);
std::string toString(const AstValue *ref);
std::string toString(const struct tpt_column_base *tpt_base,
                     const struct dpt_column *dpt_column, struct dbms *dbms);
// std

const void *getAstValuePtr(AstValue *ref);


const struct fast_unop_func *toDbmsFunc(OperationType optype, table_column_type dtype);
const struct fast_binop_func *toDbmsFunc(OperationType optype, table_column_type left,
                                         table_column_type right);
