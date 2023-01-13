#pragma once

#include <dbpb/ast.pb.h>
#include <qpg/ast.hpp>

namespace arc::qpg::proto {

void serialize(const Ast &from, dbpb::AstSubclass &to);

Ast *deserialize(const dbpb::AstSubclass &from);

}// namespace arc::qpg::proto
