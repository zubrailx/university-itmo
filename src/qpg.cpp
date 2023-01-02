#include <qpg.hpp>

#include "parser.hpp"
#include "lexer.hpp"

void parse_input(std::string str) {
  yy_scan_string(str.c_str());
  yyparse();
}
