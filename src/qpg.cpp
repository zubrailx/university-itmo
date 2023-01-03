#include <qpg.hpp>

#include "lexer.hpp"
#include "parser.hpp"
#include <ast.hpp>

extern Ast* parsed_result;

std::unique_ptr<Ast> parse_input(const std::string& str) {
  yy_scan_string(str.c_str());
  yyparse();
  yylex_destroy();
  return std::unique_ptr<Ast>(parsed_result);
}
