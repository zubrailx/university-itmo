#include <qpg/qpg.hpp>
#include <qpg/ast.hpp>

#include "parser.hpp"
#include "lexer.hpp"

int parse_input(const std::string &str, AstWrapper &res) {
  yy_scan_string(str.c_str());
  int code = yyparse(res);
  yylex_destroy();
  return code;
}
