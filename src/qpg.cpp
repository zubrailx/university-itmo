#include <qpg.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include <ast.hpp>

int parse_input(const std::string &str, AstWrapper &res) {
  yy_scan_string(str.c_str());
  int code = yyparse(res);
  yylex_destroy();
  return code;
}
