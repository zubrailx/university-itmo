#include <arc/qpg/proto.hpp>
#include <dbpb/ast.pb.h>

void arc::qpg::proto::serialize(const Ast &from, dbpb::AstSubclass &to) {}

Ast *arc::qpg::proto::deserialize(const dbpb::AstSubclass &from) {}
